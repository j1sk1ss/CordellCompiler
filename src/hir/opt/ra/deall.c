/*
deall.c - Create deallocation points in HIR
*/

#include <hir/opt/ra.h>
#define DEF_OUT_DEALLOC
int HIR_RA_create_deall(cfg_ctx_t* cctx, igraph_t* g, map_t* colors) {
    list_iter_t fit;
    list_iter_hinit(&cctx->funcs, &fit);
    cfg_func_t* fb;
    while ((fb = (cfg_func_t*)list_iter_next(&fit))) {
        list_iter_t bit;
        list_iter_hinit(&fb->blocks, &bit);
        cfg_block_t* cb;
        while ((cb = (cfg_block_t*)list_iter_next(&bit))) {
#ifdef DEF_OUT_DEALLOC
            set_iter_t init;
            set_iter_init(&cb->curr_in, &init);
            long vid;
            while ((vid = set_iter_next_int(&init)) >= 0) {
                if (set_has_int(&cb->curr_out, vid)) continue;
                HIR_insert_block(HIR_create_block(HIR_VRDEALL, HIR_SUBJ_CONST(vid), NULL, NULL), cb->exit);
            }
#else
            hir_block_t* hh = cb->entry;
            while (hh) {
                long vid = -1;
                if (hh->op == HIR_PHI && HIR_is_vartype(hh->sarg->t))        vid = hh->sarg->storage.var.v_id;
                else if (HIR_writeop(hh->op) && HIR_is_vartype(hh->farg->t)) vid = hh->farg->storage.var.v_id;
                
                if (vid >= 0) {
                    long vclr = (long)map_get(colors, vid);
                    list_iter_t nit;
                    list_iter_hinit(&g->nodes, &nit);
                    igraph_node_t* n;
                    while ((n = (igraph_node_t*)list_iter_next(&nit))) {
                        if (n->v_id == vid) {
                            n->used = 1;
                            continue;
                        }

                        if (n->color != vclr) continue;
                        if (n->used) {
                            HIR_insert_block(HIR_create_block(HIR_VRDEALL, HIR_SUBJ_CONST(n->v_id), NULL, NULL), hh);
                            n->used = 0;
                            break;
                        }
                    }
                }

                if (hh == cb->exit) break;
                hh = hh->next;
            }
#endif
        }
    }
}
