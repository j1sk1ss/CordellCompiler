/* Tail Call Optimisation (TCO)
The idea behind this module is to eliminate calls at ends of
terminator blocks.

The problem why we need this is that assembly's `call` and `ret` are
expensive. Let's eliminate them! */
#include <hir/func.h>

int HIR_FUNC_perform_tco(cfg_ctx_t* cctx, sym_table_t* smt) {
    foreach (cfg_func_t* fb, &cctx->funcs) {
        foreach (cfg_block_t* cb, &fb->blocks) {
            if (set_size(&cb->succ)) continue;
        }
    }

    return 1;
}
