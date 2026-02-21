#include <lir/selector/x84_64_gnu_nasm.h>

/*
Collect used registers in profided function.
Params:
    - `dirty` - Output set of used registers.
    - `f` - Function CFG.

Return 1 if operation succeed. Otherwise will return 0.
*/
static int _collect_in_function_reg_usage(set_t* dirty, cfg_func_t* f) {
    if (!f) return 0;
    foreach (cfg_block_t* bb, &f->blocks) {
        lir_block_t* lh = LIR_get_next(bb->lmap.entry, bb->lmap.exit, 0);
        while (lh) {
            if (
                LIR_writeop(lh->op) &&      /* We are writing some value to register (for some reason) */
                lh->farg->t == LIR_REGISTER /* This is a register object                               */
            ) set_add(dirty, (void*)lh->farg->storage.reg.reg); /* We re-write value in a register     */
            lh = LIR_get_next(lh, bb->lmap.exit, 1);
        }
    }

    return 0;
}

static unsigned int _visit_counter = 10; /* Magic index offset. TODO */

/*
Collect register usage in the further CFG.
Params:
    - `dirty` - Previously rewritten registers.
    - `save` - Output set.
    - `bbh` - Current BasicBlock.
    - `off` - Lir block off.

Return 1 if operation succeed. Otherwise will return 0.
*/
static int _collect_out_function_reg_usage(set_t* dirty, set_t* save, cfg_block_t* bbh, lir_block_t* off) {
    if (!bbh || !set_size(dirty)) return 0;
    if (bbh->visited != _visit_counter) bbh->visited = _visit_counter;
    else return 0;
    
    lir_block_t* lh = off ? off : bbh->lmap.entry;
    while (lh) {
        if (
            LIR_writeop(lh->op) && 
            lh->farg == LIR_REGISTER
        ) { /* Remove register from the 'dirty' set if it is rewritten */
            set_remove(dirty, (void*)lh->farg->storage.reg.reg);
        }
        
        lir_subject_t* args[3] = { lh->farg, lh->sarg, lh->targ };
        for (int i = LIR_writeop(lh->op); i < 3; i++) {
            if (
                !args[i] || args[i]->t != LIR_REGISTER || 
                !set_has(dirty, (void*)args[i]->storage.reg.reg)
            ) continue; /* If this register isn't dirty -> skip it */
            set_add(save, (void*)args[i]->storage.reg.reg);
        }
        
        lh = LIR_get_next(lh, bbh->lmap.exit, 1);
    }

    set_t copy;

    set_copy(&copy, dirty);
    _collect_out_function_reg_usage(&copy, save, bbh->l, off);
    set_free(&copy);
    
    set_copy(&copy, dirty);
    _collect_out_function_reg_usage(&copy, save, bbh->jmp, off);
    set_free(&copy);

    return 0;
}

/*
Find function by the provided ID.
Params:
    - `fid` - Function ID.
    - `cctx` - CFG context.

Returns function or NULL.
*/
static cfg_func_t* _find_function(long fid, cfg_ctx_t* cctx) {
    foreach (cfg_func_t* fb, &cctx->funcs) { if (fb->fid == fid) return fb; }
    return NULL;
}

int x86_64_gnu_nasm_caller_saving(cfg_ctx_t* cctx) {
    foreach (cfg_func_t* fb, &cctx->funcs) {
        if (!fb->used) continue;
        foreach (cfg_block_t* bb, &fb->blocks) {
            lir_block_t* lh = LIR_get_next(bb->lmap.entry, bb->lmap.exit, 0);
            while (lh) {
                if (lh->op == LIR_FCLL) {
                    set_t func_regs, save_regs;
                    set_init(&func_regs, SET_NO_CMP);
                    set_init(&save_regs, SET_NO_CMP);
                    
                    _visit_counter++;
                    _collect_in_function_reg_usage(&func_regs, _find_function(lh->farg->storage.str.sid, cctx));
                    _collect_out_function_reg_usage(&func_regs, &save_regs, bb, lh);
                    set_foreach (long reg, &save_regs) {
                        LIR_insert_block_before(LIR_create_block(LIR_PUSH, LIR_SUBJ_REG(reg, DEFAULT_TYPE_SIZE), NULL, NULL), lh);
                        LIR_insert_block_after(LIR_create_block(LIR_POP, LIR_SUBJ_REG(reg, DEFAULT_TYPE_SIZE), NULL, NULL), lh);
                    }

                    set_free(&func_regs);
                    set_free(&save_regs);
                }
            
                lh = LIR_get_next(lh, bb->lmap.exit, 1);
            }
        }
    }

    return 1;
}
