#include <hir/loop.h>

static inline int _has_exit(loop_node_t* loop, cfg_block_t* bb) {
    set_foreach (cfg_block_t* succ, &bb->succ) {
        if (!set_has(&loop->blocks, succ)) return 1;
    }

    return 0;
}

int HIR_LOOP_get_cond_blocks(loop_node_t* loop, set_t* conds) {
    set_foreach (cfg_block_t* bb, &loop->blocks) {
        if (
            !bb->hmap.exit || bb->hmap.exit->op != HIR_IFOP2 ||
            !_has_exit(loop, bb)
        ) continue;

        if (!set_has(conds, bb) && !set_add(conds, bb)) return 0;
    }

    return 1;
}
