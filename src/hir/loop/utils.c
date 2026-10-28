#include <hir/loop.h>

static inline int _has_exit(cfg_block_t* bb) {
    set_foreach (cfg_block_t* succ, &bb->succ) {
        if (succ->type != CFG_LOOP_BLOCK) return 1;
    }

    return 0;
}

int HIR_LOOP_get_conds(loop_node_t* loop, set_t* conds) {
    cfg_block_t* entry = NULL;
    foreach (entry, &loop->blocks_sorted) { // find entry cond
        if (entry->type == CFG_LOOP_HEADER) {
            set_add(conds, entry->hmap.exit);
            if (_has_exit(entry)) return 1;
        }
    }

    cfg_block_t* next_entry = entry->jmp->jmp; // find sub-cond (if this is a lazy logic operation)
    while (!_has_exit(next_entry)) {
        set_add(conds, next_entry);
        next_entry = next_entry->jmp->jmp;
    }

    return 1;
}
