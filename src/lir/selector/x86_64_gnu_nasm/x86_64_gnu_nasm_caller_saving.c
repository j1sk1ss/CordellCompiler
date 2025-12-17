#include <lir/selector/x84_64_gnu_nasm.h>

// TODO: result in RAX mustn't be rewritten!
static int _collect_in_function_reg_usage(set_t* dirty, cfg_func_t* f) {
    if (!f) return 0;
    foreach (cfg_block_t* bb, &f->blocks) {
        lir_block_t* lh = LIR_get_next(bb->lmap.entry, bb->lmap.exit, 0);
        while (lh) {
            if (
                LIR_writeop(lh->op) && 
                lh->farg->t == LIR_REGISTER
            ) set_add(dirty, (void*)lh->farg->storage.reg.reg);
            lh = LIR_get_next(lh, bb->lmap.exit, 1);
        }
    }

    return 0;
}

static unsigned int _visit_counter = 0;
static int _collect_out_function_reg_usage(set_t* dirty, set_t* save, cfg_block_t* bbh, lir_block_t* off) {
    if (!bbh || !set_size(dirty)) return 0;
    if (bbh->visited != _visit_counter) bbh->visited = _visit_counter;
    else return 0;
    
    lir_block_t* lh = off ? off : bbh->lmap.entry;
    while (lh) {
        if (LIR_writeop(lh->op) && lh->farg == LIR_REGISTER) {
            set_remove(dirty, (void*)lh->farg->storage.reg.reg);
        }
        
        lir_subject_t* args[3] = { lh->farg, lh->sarg, lh->targ };
        for (int i = LIR_writeop(lh->op); i < 3; i++) {
            if (!args[i] || args[i]->t != LIR_REGISTER || !set_has(dirty, (void*)args[i]->storage.reg.reg)) continue;
            set_add(save, (void*)args[i]->storage.reg.reg);
        }
        
        lh = LIR_get_next(lh, bbh->lmap.exit, 1);
    }

    if (
        _collect_out_function_reg_usage(dirty, save, bbh->l, off) || 
        _collect_out_function_reg_usage(dirty, save, bbh->jmp, off)
    ) return 1;
    return 0;
}

static cfg_func_t* _find_function(long fid, cfg_ctx_t* cctx) {
    foreach (cfg_func_t* fb, &cctx->funcs) {
        if (fb->fid == fid) return fb;
    }

    return NULL;
}

int x86_64_gnu_nasm_caller_saving(cfg_ctx_t* cctx) {
    foreach (cfg_func_t* fb, &cctx->funcs) {
        if (!fb->used) continue;
        foreach (cfg_block_t* bb, &fb->blocks) {
            lir_block_t* lh = LIR_get_next(bb->lmap.entry, bb->lmap.exit, 0);
            while (lh) {
                if (lh->op == LIR_FCLL) {
                    set_t in_regs, save_regs;
                    set_init(&in_regs, SET_NO_CMP);
                    set_init(&save_regs, SET_NO_CMP);
                    
                    _visit_counter++;
                    _collect_in_function_reg_usage(&in_regs, _find_function(lh->farg->storage.str.sid, cctx));
                    _collect_out_function_reg_usage(&in_regs, &save_regs, bb, lh);

                    set_foreach (long reg, &save_regs) {
                        LIR_insert_block_before(LIR_create_block(LIR_PUSH, LIR_SUBJ_REG(reg, DEFAULT_TYPE_SIZE), NULL, NULL), lh);
                        LIR_insert_block_after(LIR_create_block(LIR_POP, LIR_SUBJ_REG(reg, DEFAULT_TYPE_SIZE), NULL, NULL), lh);
                    }

                    set_free(&in_regs);
                    set_free(&save_regs);
                }
            
                lh = LIR_get_next(lh, bb->lmap.exit, 1);
            }
        }
    }

    return 1;
}
