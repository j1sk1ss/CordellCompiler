#include <lir/lirgen.h>

int LIR_generate(cfg_ctx_t* cctx, lir_ctx_t* ctx, sym_table_t* smt) {
    return LIR_generate_block(cctx, ctx, smt);
}

lir_block_t* LIR_get_next(lir_block_t* c, lir_block_t* exit, int skip) {
    while (c->next && c->next->unused && skip-- <= 0) {
        if (c == exit) return NULL;
        c = c->next;
    }

    return c->next;
}
