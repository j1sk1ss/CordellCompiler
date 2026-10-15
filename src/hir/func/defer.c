#include <hir/func.h>

typedef struct {
    list_t       blocks;
    cfg_block_t* home;
} defer_entry_t;

static int _unload_defer_entry(defer_entry_t* entry) {
    list_free(&entry->blocks);
    return mm_free(entry);
}

static int _collect_defer_blocks(cfg_block_t* bb, list_t* entries) {
    defer_entry_t* entry = (defer_entry_t*)mm_malloc(sizeof(defer_entry_t));
    if (!entry) return 0;
    list_init(&entry->blocks);
    entry->home = bb;

    int defer = 0;
    iterate_hir_instructions (bb) {
        if (
            hh->op == HIR_DEFER_START ||
            hh->op == HIR_DEFER_END
        ) {
            defer = hh->op == HIR_DEFER_START;
            hh->unused = 1;
            continue;
        }

        if (!defer) continue;
        hh->unused = 1;
        list_add(&entry->blocks, hh);
    }

    if (list_size(&entry->blocks)) list_add(entries, entry);
    else _unload_defer_entry(entry);
    return 0;
}

static inline hir_block_t* _get_last_valid_instruction(cfg_block_t* bb) {
    hir_block_t* last = bb->hmap.exit;
    while (
        last && 
        (
            HIR_is_syst(last->op) || 
            last->op == HIR_FRET  || 
            last->op == HIR_EXITOP
        )
    ) last = last->prev;
    return last;
}

int HIR_FUNC_set_defer_calls(cfg_ctx_t* cctx) {
    foreach (cfg_func_t* fb, &cctx->funcs) {
        list_t defers;
        list_init(&defers);

        int entry = 1;
        foreach (cfg_block_t* bb, &fb->blocks) {
            _collect_defer_blocks(bb, &defers);
            if (
                !set_size(&bb->succ) &&
                (set_size(&bb->pred) || entry)
            ) {
                foreach (defer_entry_t* defer, &defers) {
                    if (set_has(&bb->dom, defer->home)) {
                        hir_block_t* ancor = _get_last_valid_instruction(bb);
                        foreach (hir_block_t* defer_block, &defer->blocks) {
                            hir_block_t* copy = HIR_copy_block(defer_block, 0);
                            if (
                                ancor->op == HIR_FRET ||
                                ancor->op == HIR_EXITOP
                            ) HIR_insert_block_before(copy, ancor);
                            else {
                                HIR_insert_block_after(copy, ancor);
                                ancor = copy;
                            }
                        }
                    }
                }
            }

            entry = 0;
        }

        list_free_force_op(&defers, (int (*)(void*))_unload_defer_entry);
    }

    return 1;
}
