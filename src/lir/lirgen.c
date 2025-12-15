#include <lir/lirgen.h>

int LIR_generate(cfg_ctx_t* cctx, lir_ctx_t* ctx, sym_table_t* smt) {
    return LIR_generate_block(cctx, ctx, smt);
}

lir_block_t* LIR_get_next(lir_block_t* c, lir_block_t* exit, int skip) {
    if (c == exit) return NULL;
    while (c) {
        if (
            c == exit || 
            (!c->unused && skip-- <= 0)
        ) break;
        c = c->next;
    }

    return c;
}
