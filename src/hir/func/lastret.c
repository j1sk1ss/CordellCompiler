#include <hir/func.h>

int HIR_FUNC_set_last_return(cfg_ctx_t* cctx) {
    foreach (cfg_func_t* fb, &cctx->funcs) {
        cfg_block_t* rblock = NULL;
        int entry = 1, rcount = 0;
        foreach (cfg_block_t* bb, &fb->blocks) {
            if (!bb->l && !bb->jmp && (set_size(&bb->pred) || entry)) {
                if (rcount++ > 1) goto _skip_function;
                rblock = bb;
            }

            entry = 0;
        }
        
        if (rblock) {
            hir_block_t* hb = rblock->hmap.exit;
            while (hb && HIR_is_syst(hb->op)) hb = hb->prev;
            if (hb->op == HIR_VRUSE) {
                hb->op = HIR_FRET;
                rblock->hmap.exit = hb;
            }
        }
_skip_function: {}
    }

    return 1;
}
