#include <hir/loop.h>

static inline void _get_cond_vids(set_t* conds, set_t* out) {
    set_foreach (hir_block_t* cond, conds) {
        set_add(out, cond->farg->storage.var.v_id);
    }
}

/* Count actual work in a CFG block, ignoring structural commands.
   Before SSA/liveness, an arithmetic operation or store cannot be assumed dead:
   it may update the condition, a live-out variable, or memory.
   Params:
       - `bb` - CFG block to inspect.
   
   Returns the number of live commands. */
static inline int _count_commands(loop_node_t* root, set_t* cond_ind) {
    foreach (cfg_block_t* bb, &root->blocks_sorted) {
        if (bb->type != CFG_DEFAULT_BLOCK) continue;
        iterate_hir_instructions (bb) {
            if (
                HIR_is_funccall(hh->op) ||
                hh->op == HIR_LDREF
            ) return 1;
            else {
                iterate_hir_args (hir_subject_t* s, hh, 0) {
                    if (
                        HIR_is_vartype(s->t) && 
                        set_has(&root->ind, s->storage.var.v_id) && 
                        !set_has(cond_ind, s->storage.var.v_id)
                    ) return 1;
                }
            }
        }
    }

    return 0;
}

static int _mark_loop_dead(loop_node_t* root) {
    foreach (loop_node_t* ch, &root->children) {
        _mark_loop_dead(ch);
    }

    set_t conds, conds_vid;
    set_init(&conds, SET_NO_CMP);
    set_init(&conds_vid, SET_NO_CMP);

    HIR_LOOP_get_conds(root, &conds);
    _get_cond_vids(&conds, &conds_vid);

    int infinity_loop = 1;
    set_foreach (symbol_id_t inductive, &root->ind) {
        if (set_has(&conds_vid, inductive)) {
            infinity_loop = 0;
            break;
        }
    }
// WIP+TODO: Find labels and re-connect them with new blocks
    if (!infinity_loop && !_count_commands(root, &conds_vid)) { /* endless loop can be created on purpose */
        cfg_block_t *exit = NULL, *preheader = NULL;
        set_foreach (exit, &root->exits) {}             // We KNOW that there is one (or None) exit
        set_foreach (preheader, &root->header->pred) { 
            if (preheader->type == CFG_LOOP_PREHEADER) break; 
        } // We KNOW that there is a preheader (After LICM)

        if (exit && preheader) {
            if (!set_has(&preheader->succ, root->header)) return 0;

            set_remove(&preheader->succ, root->header);
            set_remove(&root->header->pred, preheader);

            set_add(&preheader->succ, exit);
            set_add(&exit->pred, preheader);

            preheader->l   = NULL;
            preheader->jmp = exit;
        }
    }

    set_free(&conds);
    set_free(&conds_vid);
    return 1;
}

int HIR_LOOP_perform_dle(ltree_ctx_t* lctx) {
    map_foreach (list_t* loops, &lctx->lmap) {
        foreach (loop_node_t* loop, loops) {
            _mark_loop_dead(loop);
        }
    }

    return 1;
}
