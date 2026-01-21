#include <hir/hirgen.h>

int HIR_generate(ast_ctx_t* sctx, hir_ctx_t* hctx, sym_table_t* smt) {
    return HIR_generate_block(sctx->r, hctx, smt);
}

/*
Check if this the last block that is shaped with the 'b' block.
Params:
    - `a` - Current block.
    - `b` - Last block.

Return if this is the last block in a sequence.
*/
static inline int _is_term(hir_block_t* a, hir_block_t* b) {
    return a == b;
}

hir_block_t* HIR_get_next(hir_block_t* c, hir_block_t* exit, int skip) {
    if (_is_term(c, exit)) return NULL;
    while (c) {
        if (
            _is_term(c, exit) || 
            (!c->unused && skip-- <= 0)
        ) break;
        c = c->next;
    }

    return c;
}
