#include <hir/func.h>

static int _collect_defer_blocks(cfg_func_t* fb, list_t* defer_blocks) {
    int defer = 0;
    foreach (cfg_block_t* bb, &fb->blocks) {
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
            list_add(defer_blocks, hh);
        }
    }

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
        list_t func_defer;
        list_init(&func_defer);
        _collect_defer_blocks(fb, &func_defer);

        int entry = 1;
        foreach (cfg_block_t* bb, &fb->blocks) {
            if (!bb->l && !bb->jmp && (set_size(&bb->pred) || entry)) {
                hir_block_t* ancor = _get_last_valid_instruction(bb);
                foreach (hir_block_t* defer_block, &func_defer) {
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

            entry = 0;
        }

        list_free(&func_defer);
    }

    return 1;
}
