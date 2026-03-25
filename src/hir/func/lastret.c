#include <hir/func.h>

int HIR_FUNC_set_last_return(cfg_ctx_t* cctx) {
    foreach (cfg_func_t* fb, &cctx->funcs) {
        list_t ret_blocks;
        list_init(&ret_blocks);

        int entry = 1;
        foreach (cfg_block_t* bb, &fb->blocks) {
            if (!bb->l && !bb->jmp && (set_size(&bb->pred) || entry)) {
                list_add(&ret_blocks, bb);
            }

            entry = 0;
        }
        
        if (list_size(&ret_blocks) == 1) {
            cfg_block_t* rblock = (cfg_block_t*)ret_blocks.h->data; 
            hir_block_t* hb = rblock->hmap.exit;
            while (hb && HIR_is_syst(hb->op)) hb = hb->prev;
            if (hb->op == HIR_VRUSE) hb->op = HIR_FRET;
        }

        list_free(&ret_blocks);
    }
}
