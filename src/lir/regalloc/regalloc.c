#include <lir/regalloc/regalloc.h>

int LIR_regalloc(cfg_ctx_t* cctx, sym_table_t* smt, map_t* colors) {
    igraph_t ig;
    LIR_RA_build_igraph(cctx, &ig, smt);
    LIR_RA_color_igraph(&ig, colors);
    LIR_RA_unload_igraph(&ig);
    return 1;
}
