/*
deall.c - Create deallocation points in HIR
*/

#include <hir/opt/ra.h>

int HIR_RA_create_deall(cfg_ctx_t* cctx, igraph_t* g, map_t* colors) {
    list_iter_t fit;
    list_iter_hinit(&cctx->funcs, &fit);
    cfg_func_t* fb;
    while ((fb = (cfg_func_t*)list_iter_next(&fit))) {
        list_iter_t bit;
        list_iter_hinit(&fb->blocks, &bit);
        cfg_block_t* cb;
        while ((cb = (cfg_block_t*)list_iter_next(&bit))) {
            set_iter_t defit;
            set_iter_init(&cb->def, &defit);
            long vid;
            while ((vid = set_iter_next_int(&defit)) >= 0) {
                if (set_has_int(&cb->curr_out, vid)) continue;
                HIR_insert_block(HIR_create_block(HIR_VRDEALL, HIR_SUBJ_CONST(vid), NULL, NULL), cb->exit);
            }
        }
    }
}
