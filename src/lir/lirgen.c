#include <lir/lirgen.h>

int LIR_generate(cfg_ctx_t* cctx, lir_ctx_t* ctx, sym_table_t* smt) {
    return LIR_generate_block(cctx, ctx, smt);
}

/*
Check if this the last block that is shaped with the 'b' block.
Params:
    - `a` - Current block.
    - `b` - Last block.

Return if this is the last block in a sequence.
*/
static inline int _is_term(lir_block_t* a, lir_block_t* b) {
    return a == b;
}

lir_block_t* LIR_get_next(lir_block_t* c, lir_block_t* exit, int skip) {
    if (_is_term(c, exit)) return NULL;
    for (; c && c != exit; c = c->next) {
        if (_is_term(c, exit) || skip-- == 0) return c;
        if (c->unused) continue;
    }

    return c;
}
