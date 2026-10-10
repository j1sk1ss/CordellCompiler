#include <hir/loop.h>

static int _is_op_interesting(hir_operation_t op) {
    switch (op) {
        case HIR_IFOP2:        case HIR_JMP:         case HIR_MKLB:   case HIR_MKSCOPE: case HIR_ENDSCOPE:
        case HIR_STASM:        case HIR_ENDASM:      case HIR_SETPOS: case HIR_NOP:     case HIR_PHI:
        case HIR_DEFER_END:    case HIR_DEFER_START:
        case HIR_PHI_PREAMBLE: return 1;
        default:               return 0;
    }
}

/* Count actual work in a CFG block, ignoring structural commands.
   Before SSA/liveness, an arithmetic operation or store cannot be assumed dead:
   it may update the condition, a live-out variable, or memory.
Params:
    - `bb` - CFG block to inspect.

Returns the number of live commands. */
static inline int _count_commands(cfg_block_t* bb) {
    int res = 0;
    iterate_hir_instructions (bb) {
        if (!hh->unused && (!_is_op_interesting(hh->op) || hh->op == HIR_PHI_PREAMBLE)) res++;
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
        loop_content += _count_commands(bb);
    }

    if (!loop_content) {
        set_foreach (cfg_block_t* bb, &root->blocks) {
            iterate_hir_instructions (bb) {
                if (!_is_op_interesting(hh->op)) {
                    hh->unused = 1;
                }
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
