#include <lir/regalloc/x84_64_gnu_nasm.h>

static int _regalloc_precolor(cfg_ctx_t* cctx, map_t* cmap) {
    list_iter_t fit;
    list_iter_hinit(&cctx->funcs, &fit);
    cfg_func_t* fb;
    while ((fb = (cfg_func_t*)list_iter_next(&fit))) {
        if (!fb->used) continue;
        list_iter_t bit;
        list_iter_hinit(&fb->blocks, &bit);
        cfg_block_t* bb;
        while ((bb = (cfg_block_t*)list_iter_next(&bit))) {
            lir_block_t* lh = bb->lmap.entry;
            while (lh) {
                if (lh->op == LIR_iMOV) {
                    long vid = -1, color = -1;
                    if (lh->farg->t == LIR_REGISTER && lh->sarg->t == LIR_VARIABLE) {
                        color = lh->farg->storage.reg.reg;
                        vid = lh->sarg->storage.var.v_id;
                    }
                    else if (lh->farg->t == LIR_VARIABLE && lh->sarg->t == LIR_REGISTER) {
                        color = lh->sarg->storage.reg.reg;
                        vid = lh->farg->storage.var.v_id;
                    }

                    if (vid != -1 && color != -1) {
                        LIR_RA_precolor_node(cmap, vid, color);
                    }
                }

                if (lh == bb->lmap.exit) break;
                lh = lh->next;
            }
        }
    }

    return 1;
}

int x86_64_regalloc_graph(cfg_ctx_t* cctx, sym_table_t* smt, map_t* colors) {
    igraph_t ig;
    LIR_RA_build_igraph(cctx, &ig, smt);
    _regalloc_precolor(cctx, colors);
    LIR_RA_color_igraph(&ig, colors);
    LIR_RA_unload_igraph(&ig);
    return 1;
}

int x86_64_regalloc_linear(cfg_ctx_t* cctx, sym_table_t* smt, map_t* colors) {
    return 1;
}
