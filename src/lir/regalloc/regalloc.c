#include <lir/regalloc/regalloc.h>

static int _regalloc_precolor(map_t* cmap, sym_table_t* smt) {
    map_foreach (variable_info_t* vi, &smt->v.vartb) {
        if (vi->vmi.allocated && vi->vmi.reg >= 0) {
            map_put(cmap, vi->v_id, (void*)((long)vi->vmi.reg));
        }
    }

    return 1;
}

int LIR_regalloc(cfg_ctx_t* cctx, sym_table_t* smt, map_t* colors) {
    igraph_t ig;
    LIR_RA_build_igraph(cctx, &ig, smt);
    _regalloc_precolor(colors, smt);
    LIR_RA_color_igraph(&ig, colors);
    LIR_RA_unload_igraph(&ig);
    return 1;
}
