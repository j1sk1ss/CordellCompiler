#include <hir/loop.h>

/* Count live commands in a CFG block, ignoring loop-control and inductive updates.
Params:
    - `bb` - CFG block to inspect.
    - `ind` - Set of inductive variable IDs.

Returns the number of live commands. */
static inline int _count_commands(cfg_block_t* bb, set_t* ind) {
    int res = 0;
    iterate_hir_instructions (bb) {
        switch (hh->op) {
            case HIR_IFOP2: case HIR_JMP:    case HIR_MKLB:   case HIR_MKSCOPE: case HIR_ENDSCOPE:
            case HIR_STASM: case HIR_ENDASM: case HIR_SETPOS: case HIR_NOP:     case HIR_PHI:
            case HIR_PHI_PREAMBLE: break;
            default: {
                if (
                    hh->farg && HIR_is_vartype(hh->farg->t) && 
                    set_has(ind, (void*)hh->farg->storage.var.v_id)
                ) break;
                res++;
            }
        }
    }

    return res;
}

/* Mark loop instructions as dead when the loop has no live content.
Params:
    - `root` - Loop tree node to process.

Returns 1 if succeeds. */
static int _mark_loop_dead(loop_node_t* root) {
    foreach (loop_node_t* ch, &root->children) {
        _mark_loop_dead(ch);
    }

    int loop_content = 0;
    set_foreach (cfg_block_t* bb, &root->blocks) {
        loop_content += _count_commands(bb, &root->ind);
    }

    if (!loop_content) {
        set_foreach (cfg_block_t* bb, &root->blocks) {
            iterate_hir_instructions (bb) {
                hh->unused = 1;
            }
        }
    }

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
