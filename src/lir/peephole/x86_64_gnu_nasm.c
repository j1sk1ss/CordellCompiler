#include <lir/peephole/x84_64_gnu_nasm.h>

/*
Delete jump which leads us to a fall block. We find a block which has a 
jump operation at the end. If this operation jumps to the next block,
we can remove it.
Params:
    - `bb` - Current base block.

Returns 1 if succeeds.
*/
static int _jumps_pass(cfg_block_t* bb) {
    if (!bb->lmap.exit) return 0;
    lir_block_t* l  = LIR_get_back_instruction(bb->lmap.exit, bb->lmap.entry, 0);
    lir_block_t* ll = LIR_get_back_instruction(l, bb->lmap.entry, 1);
    if (LIR_is_jumpop(l->op)) {
        cfg_block_t* next_bb = bb->l != bb ? bb->l : bb->jmp;
        if (!next_bb) return 0;
        lir_block_t* next_lh  = LIR_get_near_instruction(l, next_bb->lmap.exit, 1);
        lir_block_t* entry_ln = LIR_get_near_instruction(next_bb->lmap.entry, next_bb->lmap.exit, 0);
        if (entry_ln && entry_ln->op == LIR_MKLB && next_lh == entry_ln) {
            if (LIR_subj_equals(l->farg, entry_ln->farg)) l->unused = 1;
            if (ll && LIR_is_jumpop(ll->op) && LIR_subj_equals(ll->farg, entry_ln->farg)) ll->unused = 1;
            return 1;
        }
    }

    return 0;
}

/*
Determine whether this label is used somewhere.
Params:
    - `fb` - Function block to consider.
    - `lb` - Lable to search.

Returns 1 if the lable is used somewhere in the function.
*/
static int _find_label_usage(cfg_func_t* fb, lir_subject_t* lb) {
    lir_block_t* lh = LIR_get_next(fb->lmap.entry, fb->lmap.exit, 0);
    while (lh) {
        if (
            !lh->unused && 
            LIR_is_jumpop(lh->op) && LIR_subj_equals(lh->farg, lb)
        ) return 1;
        lh = LIR_get_next(lh, fb->lmap.exit, 1);
    }

    return 0;
}

/*
Remove all dangling labels which aren't used as a target somewhere else.
Params:
    - `fb` - Function to consideration.

Returns 1 if succeeds.
*/
static int _label_pass(cfg_func_t* fb) {
    int changed = 0;
    lir_block_t* lh = LIR_get_next(fb->lmap.entry, fb->lmap.exit, 0);
    while (lh) {
        if (
            !lh->unused && 
            lh->op == LIR_MKLB && !_find_label_usage(fb, lh->farg)
        ) {
            lh->unused = 1; 
            changed = 1;
        }

        lh = LIR_get_next(lh, fb->lmap.exit, 1);
    }

    return changed;
}

/*
Mark all LIR blocks in a CFG block as unused.
Params:
    - `bb` - CFG block to hide.
*/
static inline void _hide_block(cfg_block_t* bb) {
    lir_block_t* lh = LIR_get_next(bb->lmap.entry, bb->lmap.exit, 0);
    while (lh) {
        lh->unused = 1;
        lh = LIR_get_next(lh, bb->lmap.exit, 1);
    }
}

/*
Remove jump-only CFG blocks by redirecting their predecessors to the final
jump target and hiding the intermediate block.
Params:
    - `fb` - Function to consideration.

Returns 1 if CFG was changed, otherwise 0.
*/
static int _deep_jump_pass(cfg_func_t* fb) {
    int changed = 0;
    foreach (cfg_block_t* bb, &fb->blocks) {
        int only_lb_and_jump = 1;
        lir_block_t* lh = LIR_get_near_instruction(bb->lmap.entry, bb->lmap.exit, 0);
        while (lh) {
            if (
                !lh->unused && 
                lh->op != LIR_MKLB && lh->op != LIR_JMP
            ) {
                only_lb_and_jump = 0;
                break;
            }

            lh = LIR_get_near_instruction(lh, bb->lmap.exit, 1);
        }
        
        lir_block_t* entry_lb  = LIR_get_near_instruction(bb->lmap.entry, bb->lmap.exit, 0);
        lir_block_t* last_jump = LIR_get_back_instruction(bb->lmap.exit, bb->lmap.entry, 0);
        if (
            only_lb_and_jump && 
            entry_lb && entry_lb->op == LIR_MKLB &&
            last_jump && last_jump->op == LIR_JMP
        ) { 
            lir_subject_t* pdst = entry_lb->farg; 
            lir_subject_t* ndst = last_jump->farg; 
            cfg_block_t* bb_dst = bb->l ? bb->l : bb->jmp;
            set_foreach (cfg_block_t* prev, &bb->pred) {
                lir_block_t* l  = LIR_get_back_instruction(prev->lmap.exit, prev->lmap.entry, 0);
                lir_block_t* ll = LIR_get_back_instruction(l, prev->lmap.entry, 1);
                if (l && LIR_is_jumpop(l->op) && LIR_subj_equals(l->farg, pdst)) l->farg = ndst;
                else if (ll && LIR_is_jumpop(ll->op) && LIR_subj_equals(ll->farg, pdst)) ll->farg = ndst; 
                else continue;

                changed = 1;
                if (prev->l == bb) prev->l = bb_dst;
                if (prev->jmp == bb) prev->jmp = bb_dst;
                if (bb_dst) {
                    set_remove(&bb_dst->pred, bb);
                    set_add(&bb_dst->pred, prev);
                }
            }

            _hide_block(bb);
            bb->l = bb->jmp = NULL;
        }
    }

    return changed;
}

static unsigned long long _visit_counter = 100;

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
                lh != ign &&
                lh->op == op &&                           /* With the same operation such as mov, add, etc.               */
                LIR_subj_equals(lh->farg, trg) &&         /* And similar destination of the write operation               */
                (
                    !LIR_subj_equals(lh->sarg, trg) &&    /* The second and the third arguments must be a uniq /          */
                    !LIR_subj_equals(lh->targ, trg)       /* different with the firts.                                    */
                )                                         /* The reason is easy: We don't want to delete commad if its    */
                                                          /* value rewritten by itself.                                   */
            ) return 1;                                   /* That means we can safely mark the target write command       */

            if (
                LIR_is_readop(lh->op) &&                  /* If this instruction reads second and third arguments         */
                (
                    LIR_subj_equals(lh->farg, trg) ||     /* And either the first argument is equal to the target         */
                    LIR_subj_equals(lh->sarg, trg) ||     /* or the second argument is equal to the target.               */
                    LIR_subj_equals(lh->targ, trg)        /* Also we need to take care about the third argument too.      */
                )
            ) return 0;                                   /* That means, we should mark the target write command as valid */
        }
        
        lh = LIR_get_next(lh, bbh->lmap.exit, 1);
    }

    if (
        !_recursive_cleanup(op, bbh->id, bbh->l, trg, ign, NULL) || 
        !_recursive_cleanup(op, bbh->id, bbh->jmp, trg, ign, NULL)
    ) return 0; /* If the command is used somewhere in the childs, return 0                       */
    return 1;   /* By default, if the considering command is unused elsewhere, we mark it to drop */
}

static int _cleanup_pass(cfg_block_t* bb) {
    int changed = 0;
    if (!bb) return 0;
    lir_block_t* lh = LIR_get_next(bb->lmap.entry, bb->lmap.exit, 0);
    while (lh) {
        if (
            !lh->unused &&
            LIR_is_writeop(lh->op) && !LIR_has_sideeffect(lh->op)
        ) {
            _visit_counter++;
            if (_recursive_cleanup(lh->op, -1, bb, lh->farg, lh, lh->next)) {
                lh->unused = 1;
                changed = 1;
            }
        }

        lh = LIR_get_next(lh, bb->lmap.exit, 1);
    }

    return changed;
}

int x86_64_gnu_nasm_peephole_optimization(cfg_ctx_t* cctx) {
    int changed = 0;
    foreach (cfg_func_t* fb, &cctx->funcs) {
        if (!fb->used) continue;
        changed |= _deep_jump_pass(fb);
        foreach (cfg_block_t* bb, &fb->blocks) {
            changed |= _jumps_pass(bb);
            changed |= _cleanup_pass(bb);
        }

        changed |= _label_pass(fb);
    }

    return changed;
}
