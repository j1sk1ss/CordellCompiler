#include <lir/peephole/x84_64_gnu_nasm.h>

/*
Simple function that helps us with a 'home verification' proccess.
Params:
    - `h` - Expected home of the subject.
    - `s` - Considering subject.

Return 1 if the 'h' is a home for the 's'.
*/
static int _check_home(lir_block_t* h, lir_subject_t* s) {
    if (!h || !s) return 0;
    lir_subject_t* args[] = { h->farg, h->sarg, h->targ };
    for (int i = 0; i < 3; i++) {
        if (args[i] && args[i] == s) return 1;
    }

    return 0;
}

static int _jumps_pass(cfg_block_t* bb) {
    if (!bb->lmap.exit) return 0;
    lir_block_t* l  = LIR_get_back_instruction(bb->lmap.exit, bb->lmap.entry, 0);
    lir_block_t* ll = LIR_get_back_instruction(l, bb->lmap.entry, 1);
    if (LIR_is_jumpop(l->op)) {
        cfg_block_t* next_bb  = bb->l != bb ? bb->l : bb->jmp;
        if (!next_bb) return 0;
        lir_block_t* next_lh  = LIR_get_near_instruction(l, next_bb->lmap.exit, 1);
        lir_block_t* entry_ln = LIR_get_near_instruction(next_bb->lmap.entry, next_bb->lmap.exit, 0);
        if (entry_ln && entry_ln->op == LIR_MKLB && next_lh == entry_ln) {
            if (LIR_subj_equals(l->farg, entry_ln->farg)) l->unused = 1;
            if (ll && LIR_is_jumpop(ll->op) && LIR_subj_equals(ll->farg, entry_ln->farg)) ll->unused = 1;
        }
    }

    return 1;
}

static int _find_label_usage(cfg_func_t* fb, lir_subject_t* lb) {
    lir_block_t* lh = LIR_get_next(fb->lmap.entry, fb->lmap.exit, 0);
    while (lh) {
        if (!lh->unused && LIR_is_jumpop(lh->op) && LIR_subj_equals(lh->farg, lb)) return 1;
        lh = LIR_get_next(lh, fb->lmap.exit, 1);
    }

    return 0;
}

static int _label_pass(cfg_func_t* fb) {
    lir_block_t* lh = LIR_get_next(fb->lmap.entry, fb->lmap.exit, 0);
    while (lh) {
        if (!lh->unused && lh->op == LIR_MKLB && !_find_label_usage(fb, lh->farg)) lh->unused = 1; 
        lh = LIR_get_next(lh, fb->lmap.exit, 1);
    }

    return 1;
}

static unsigned int _visit_counter = 100;

/*
Recursive cleanup will clean each block from the CFG with one simple rule:
If there is a WRITE operation, it may be eliminated, if:
    - Either further code doesn't use it's value or /
    - further code rewrites it's value.

Params:
    - `op` - Write operation type.
             Note: We don't use a general term of WRITE operation 
                   given a different nature of some operations.
    - `pred` - Service argument. For initial call use '-1'.
    - `bbh` - Head Basic Block.
    - `trg` - Target WRITE location in the considering lir block.
    - `ign` - Service argument. For initial call use a parent of the 'trg' argument.
    - `off` - Service argument. For initial call use 'ign->next'.

Return 1 if the considering lir block can be marked as unused.
Retrun 0 if the considering lir block can't be marked as unused.
*/
static int _recursive_cleanup(
    lir_operation_t op, long pred, cfg_block_t* bbh, lir_subject_t* trg, lir_block_t* ign, lir_block_t* off
) {
    if (!bbh) return 1;
    if (bbh->visited != _visit_counter) {
        set_free(&bbh->visitors);
        set_init(&bbh->visitors, SET_NO_CMP);
    }
    
    if (set_has(&bbh->visitors, (void*)pred)) return 1;
    bbh->visited = _visit_counter;
    set_add(&bbh->visitors, (void*)pred);

    lir_block_t* lh = off ? off : bbh->lmap.entry;
    while (lh) {
        if (!lh->unused) {
            if (                                          
                lh != ign &&                              /* If this isn't an ignored (likely the source) command         */
                lh->op == op &&                           /* With the same operation such as mov, add, etc.               */
                LIR_subj_equals(lh->farg, trg) &&         /* And similar destination of the write operation               */
                (
                    !LIR_subj_equals(lh->sarg, trg) &&    /* The second and the third arguments must be a uniq /          */
                    !LIR_subj_equals(lh->targ, trg)       /* different with the firts.                                    */
                )                                         /* The reason is easy: We don't want to delete commad if its    */
                                                        /* value rewritten by itself.                                   */
            ) return 1;                                   /* That means we can safely mark the target write command       */

            if (
                LIR_has_sideeffect(lh->op) ||             /* Skip reserved instruction                                    */
                (
                    LIR_is_readop(lh->op) &&              /* If this instruction reads second and third arguments         */
                    (
                        LIR_subj_equals(lh->farg, trg) || /* And either the first argument is equal to the target         */
                        LIR_subj_equals(lh->sarg, trg) || /* or the second argument is equal to the target.               */
                        LIR_subj_equals(lh->targ, trg)    /* Also we need to take care about the third argument too.      */
                    )
                )
            ) return 0;                                   /* That means, we should mark the target write command as valid */
        }
        
        lh = LIR_get_next(lh, bbh->lmap.exit, 1);
    }

    if (
        bbh->l && !_recursive_cleanup(op, bbh->id, bbh->l, trg, ign, NULL) || 
        bbh->jmp && !_recursive_cleanup(op, bbh->id, bbh->jmp, trg, ign, NULL)
    ) return 0; /* If the command is used somewhere in the childs, return 0                       */
    return 1;   /* By default, if the considering command is unused elsewhere, we mark it to drop */
}

static int _cleanup_pass(cfg_block_t* bb) {
    if (!bb) return 0;
    lir_block_t* lh = LIR_get_next(bb->lmap.entry, bb->lmap.exit, 0);
    while (lh) {
        if (
            LIR_is_writeop(lh->op) &&   /* If this is a write operation */
            !LIR_has_sideeffect(lh->op) /* but isn't a reserved one     */
        ) {
            _visit_counter++;
            if (_recursive_cleanup(lh->op, -1, bb, lh->farg, lh, lh->next)) {
                lh->unused = 1;
            }
        }

        lh = LIR_get_next(lh, bb->lmap.exit, 1);
    }

    return 1;
}

int x86_64_gnu_nasm_peephole_optimization(cfg_ctx_t* cctx) {
    foreach (cfg_func_t* fb, &cctx->funcs) {
        if (!fb->used) continue;
        foreach (cfg_block_t* bb, &fb->blocks) {
            _jumps_pass(bb);
            _cleanup_pass(bb);
        }
        _label_pass(fb);
    }

    return 1;
}
