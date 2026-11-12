#include <hir/loop.h>

static void _get_used_vids(hir_subject_t* arg, set_t* out) {
    if (!arg) return;
    if (HIR_is_vartype(arg->t)) set_add(out, (void*)arg->storage.var.v_id);
    else if (arg->t == HIR_ARGLIST) {
        foreach (hir_subject_t* s, &arg->storage.list.h) {
            _get_used_vids(s, out);
        }
    }
    else if (arg->t == HIR_PHISET) {
        set_foreach (int_tuple_t* incoming, &arg->storage.set.h) {
            set_add(out, (void*)incoming->y);
        }
    }
}

static cfg_dfs_action_t _collect_uses_after_loop(cfg_block_t* bb, long pred, loop_node_t* loop, set_t* out) {
    (void)pred;
    if (set_has(&loop->blocks, bb)) return CFG_DFS_SKIP;
    iterate_hir_instructions (bb) {
        if (hh->op == HIR_PHI) {
            if (hh->targ) _get_used_vids(hh->targ, out);
            continue;
        }

        int offset = HIR_is_writeop(hh->op) || hh->op == HIR_PHI_PREAMBLE;
        iterate_hir_args (hir_subject_t* arg, hh, offset) {
            _get_used_vids(arg, out);
        }
    }

    return CFG_DFS_CONTINUE;
}

static int _cond_used_after_loop(loop_node_t* loop, set_t* cond_vids) {
    set_t used;
    if (!set_init(&used, SET_NO_CMP)) return 1;

    int live = 0;
    big_id_t counter = CFG_get_unique_counter();
    set_foreach (cfg_block_t* exit, &loop->exits) {
        if (!CFG_DFS_WALK_COUNTER(exit, -1, counter, _collect_uses_after_loop, loop, &used)) {
            live = 1;
            break;
        }
    }

    set_foreach (void* vid, cond_vids) {
        if (set_has(&used, vid)) {
            live = 1;
            break;
        }
    }

    set_free(&used);
    return live;
}

static inline int _count_commands(loop_node_t* root, set_t* cond_ind) {
    set_foreach (cfg_block_t* bb, &root->blocks) {
        iterate_hir_instructions (bb) {
            switch (hh->op) {
                case HIR_NOP: case HIR_MKLB: case HIR_MKSCOPE: case HIR_ENDSCOPE:
                case HIR_JMP: case HIR_IFOP2: continue;
                case HIR_PHI: {
                    if (
                        !hh->sarg ||
                        !set_has(cond_ind, (void*)hh->sarg->storage.var.v_id)
                    ) return 1;
                    continue;
                }
                case HIR_PHI_PREAMBLE:
                case HIR_STORE: case HIR_iADD: case HIR_iSUB:
                case HIR_iLWR: case HIR_iLRE: case HIR_iLRG: case HIR_iLGE:
                case HIR_iCMP: case HIR_iNMP:
                case HIR_TI8: case HIR_TI16: case HIR_TI32: case HIR_TI64:
                case HIR_TU8: case HIR_TU16: case HIR_TU32: case HIR_TU64: break;
                default: return 1;
            }

            iterate_hir_args (hir_subject_t* s, hh, 0) {
                if (
                    HIR_is_vartype(s->t) &&
                    (HIR_is_global(s->t) || s->ptr || !set_has(cond_ind, (void*)s->storage.var.v_id))
                ) return 1;
            }
        }
    }

    return 0;
}

static int _mark_loop_dead(loop_node_t* root, sym_table_t* smt) {
    foreach (loop_node_t* ch, &root->children) {
        if (!_mark_loop_dead(ch, smt)) return 0;
    }

    loop_iteration_info_t iterations;
    if (!HIR_LOOP_analyze_iterations(root, &iterations, smt)) {
        HIR_LOOP_free_iteration_info(&iterations);
        return 0;
    }

    if (
        iterations.status == LOOP_ITERATION_FINITE &&                      /* endless loop can be created on purpose */
        !_count_commands(root, &iterations.iteration_dependency_ids) &&    /* there is no useful commands            */
        !_cond_used_after_loop(root, &iterations.iteration_dependency_ids) /* inductive variables don't used below   */
    ) {
        cfg_block_t *exit = NULL, *preheader = NULL;
        set_foreach (exit, &root->exits) {}             // We KNOW that there is one (or None) exit
        set_foreach (cfg_block_t* pred, &root->header->pred) {
            if (pred->type == CFG_LOOP_PREHEADER) preheader = pred;
        } // We KNOW that there is a preheader (After LICM)

        if (exit && preheader) {
            if (!set_has(&preheader->succ, root->header)) {
                HIR_LOOP_free_iteration_info(&iterations);
                return 0;
            }

            set_remove(&preheader->succ, root->header);
            set_remove(&root->header->pred, preheader);

            set_add(&preheader->succ, exit);
            set_add(&exit->pred, preheader);

            preheader->l   = NULL;
            preheader->jmp = exit;
        }
    }

    HIR_LOOP_free_iteration_info(&iterations);
    return 1;
}

int HIR_LOOP_perform_dle(ltree_ctx_t* lctx, sym_table_t* smt) {
    map_foreach (list_t* loops, &lctx->lmap) {
        foreach (loop_node_t* loop, loops) {
            if (!_mark_loop_dead(loop, smt)) return 0;
        }
    }

    return 1;
}
