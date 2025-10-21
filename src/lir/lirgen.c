#include <lir/lirgen.h>

int LIR_generate(hir_ctx_t* hctx, lir_gen_t* g, lir_ctx_t* ctx, sym_table_t* smt) {
    int res = g->generate(hctx, ctx, smt);
    if (g->mvclean) g->mvclean(ctx);
    return res;
}
