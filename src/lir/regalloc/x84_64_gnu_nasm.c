#include <lir/regalloc/x84_64_gnu_nasm.h>

static int _x86_64_GC_regalloc_precolor(cfg_ctx_t* cctx, map_t* cmap) {
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
                    
                }

                if (lh == bb->lmap.exit) break;
                lh = lh->next;
            }
        }
    }

    return 1;
}

int x86_64_regalloc_graph(cfg_ctx_t* cctx, sym_table_t* smt) {
    igraph_t ig;
    LIR_RA_build_igraph(cctx, &ig, &smt);

    map_t clrs;
    map_init(&clrs);

    _x86_64_GC_regalloc_precolor(cctx, &clrs);
    LIR_RA_color_igraph(&ig, &clrs);

    return 1;
}

int x86_64_regalloc_linear(cfg_ctx_t* cctx) {
    return 1;
}
