#include <lir/lirgen.h>

int LIR_generate(cfg_ctx_t* cctx, lir_ctx_t* ctx, sym_table_t* smt) {
    return LIR_generate_block(cctx, ctx, smt);
}

lir_block_t* LIR_get_next(lir_block_t* c, lir_block_t* exit, int skip) {
    if (!skip) return c;
    if (c == exit) return NULL;
    for (; c && c != exit; c = c->next) {
        if ((c == exit) || skip-- == 0) return c;
        if (c->unused || c->op == LIR_BB) continue;
    }

    return c;
}
