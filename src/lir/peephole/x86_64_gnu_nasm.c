#include <lir/peephole/x86_64_gnu_nasm.h>

/* Delete jump which leads us to a fall block. We find a block which has a 
jump operation at the end. If this operation jumps to the next block,
we can remove it.
Params:
    - `bb` - Current base block.

Returns 1 if succeeds */
static int _jumps_pass(cfg_block_t* bb) {
    if (!bb->lmap.exit) return 0;
    lir_block_t* l  = LIR_get_back_instruction(bb->lmap.exit, bb->lmap.entry, 0);
    lir_block_t* ll = LIR_get_back_instruction(l, bb->lmap.entry, 1);
    if (LIR_is_jumpop(l->op)) {
        cfg_block_t* next_bb = (bb->l && bb->l != bb) ? bb->l : bb->jmp;
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

/* Determine whether this label is used somewhere.
Params:
    - `fb` - Function block to consider.
    - `lb` - Label to search.

Returns 1 if the label is used somewhere in the function */
static int _find_label_usage(cfg_func_t* fb, lir_subject_t* lb) {
    iterate_lir_instructions (fb) {
        if (
            !lh->unused && 
            LIR_is_jumpop(lh->op) && LIR_subj_equals(lh->farg, lb)
        ) return 1;
    }

    return 0;
}

/* Remove all dangling labels which aren't used as a target somewhere else.
Params:
    - `fb` - Function to consider.

Returns 1 if succeeds */
static int _label_pass(cfg_func_t* fb) {
    int changed = 0;
    iterate_lir_instructions (fb) {
        if (
            !lh->unused && 
            lh->op == LIR_MKLB && !_find_label_usage(fb, lh->farg)
        ) lh->unused = changed = 1;
    }

    return changed;
}

/* Remove jump-only CFG blocks by redirecting their predecessors to the final
jump target and hiding the intermediate block.
Params:
    - `fb` - Function to consider.

Returns 1 if CFG was changed, otherwise 0 */
static int _deep_jump_pass(cfg_func_t* fb) {
    int changed = 0;
    foreach (cfg_block_t* bb, &fb->blocks) {
        int only_lb_and_jump = 1;
        iterate_lir_instructions (bb) {
            if (
                !lh->unused && 
                lh->op != LIR_MKLB && lh->op != LIR_JMP
            ) {
                only_lb_and_jump = 0;
                break;
            }
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

            iterate_lir_instructions (bb) {
                lh->unused = 1;
            }

            bb->l = bb->jmp = NULL;
        }
    }

    return changed;
}

static int _is_stack_pointer_subject(lir_subject_t* s) {
    return s && s->t == LIR_REGISTER && LIR_format_register(s->storage.reg.reg, 8) == RSP;
}

static cfg_dfs_action_t _cleanup_walk_logic(
    cfg_block_t* bbh, long pred, lir_operation_t op, lir_subject_t* trg, lir_block_t* ign, lir_block_t* off
) {
    lir_block_t* lh = pred == -1 && off ? off : bbh->lmap.entry;
    while (lh) {
        if (!lh->unused) {
            if (
                lh != ign &&
                lh->op == op &&                           /* With the same operation such as mov, add, etc.               */
                LIR_subj_equals(lh->farg, trg) &&         /* And similar destination of the write operation               */
                (
                    !LIR_subj_equals(lh->sarg, trg) &&    /* The second and the third arguments must be a uniq /          */
                    !LIR_subj_equals(lh->targ, trg)       /* different with the first.                                    */
                )                                         /* The reason is easy: We don't want to delete command if its   */
                                                          /* value rewritten by itself.                                   */
            ) return CFG_DFS_SKIP;                        /* That means we can safely mark the target write command       */
            if (
                LIR_is_readop(lh->op) &&                  /* If this instruction reads second and third arguments         */
                (
                    LIR_subj_equals(lh->farg, trg) ||     /* And either the first argument is equal to the target         */
                    LIR_subj_equals(lh->sarg, trg) ||     /* or the second argument is equal to the target.               */
                    LIR_subj_equals(lh->targ, trg)        /* Also we need to take care about the third argument too.      */
                )
            ) return CFG_DFS_STOP;                        /* That means, we should mark the target write command as valid */
        }
        
        lh = LIR_get_next(lh, bbh->lmap.exit, 1);
    }

    return CFG_DFS_CONTINUE;
}

static int _cleanup_pass(cfg_block_t* bb) {
    int changed = 0;
    if (!bb) return 0;
    iterate_lir_instructions (bb) {
        if (
            !lh->unused &&
            !_is_stack_pointer_subject(lh->farg) &&
            LIR_is_writeop(lh->op) && !LIR_has_sideeffect(lh->op)
        ) {
            if (CFG_DFS_WALK(bb, _cleanup_walk_logic, lh->op, lh->farg, lh, lh->next)) {
                lh->unused = 1;
                changed = 1;
            }
        }
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
