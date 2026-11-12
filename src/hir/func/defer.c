#include <hir/func.h>

typedef struct {
    list_t       blocks;
    cfg_block_t* home;
} defer_entry_t;

static int _unload_defer_entry(defer_entry_t* entry) {
    list_free(&entry->blocks);
    return mm_free(entry);
}

static status_code_t _collect_defer_blocks(cfg_block_t* bb, sstack_t* entries) {
    int defer = 0;
    defer_entry_t* entry = NULL;
    iterate_hir_instructions (bb) {
        if (
            hh->op == HIR_DEFER_START ||
            hh->op == HIR_DEFER_END
        ) {
            defer = hh->op == HIR_DEFER_START;
            if (defer) {
                entry = (defer_entry_t*)mm_malloc(sizeof(defer_entry_t));
                if (!entry) return GENERAL_ERROR;
                list_init(&entry->blocks);
                entry->home = bb;
            }
            else {
                if (list_size(&entry->blocks)) stack_push(entries, entry);
                else                           _unload_defer_entry(entry);
            }

            hh->unused = 1;
            continue;
        }

        if (!defer) continue;
        hh->unused = 1;
        list_add(&entry->blocks, hh);
    }

    return GENERAL_SUCCESS;
}

static inline hir_block_t* _get_last_valid_instruction(cfg_block_t* bb) {
    hir_block_t* last = bb->hmap.exit;
    while (
        last &&
        (
            last->unused ||
            (HIR_is_syst(last->op) || last->op == HIR_FRET || last->op == HIR_EXITOP)
        )
    ) last = last->prev;
    return last;
}

status_code_t HIR_FUNC_set_defer_calls(cfg_ctx_t* cctx) {
    foreach (cfg_func_t* fb, &cctx->funcs) {
        sstack_t defers;
        stack_init(&defers);

        int entry = 1;
        foreach (cfg_block_t* bb, &fb->blocks) {
            _collect_defer_blocks(bb, &defers);
            if (
                !set_size(&bb->succ) &&
                (set_size(&bb->pred) || entry)
            ) {
                hir_block_t* ancor = _get_last_valid_instruction(bb);
                for (int i = defers.top; i >= 0; i--) {
                    defer_entry_t* defer = (defer_entry_t*)defers.data[i].d;
                    if (set_has(&bb->dom, defer->home)) {
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

        stack_free_force_op(&defers, (int (*)(void*))_unload_defer_entry);
    }

    return GENERAL_SUCCESS;
}
