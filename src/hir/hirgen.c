#include <hir/hirgen.h>

int HIR_generate(ast_ctx_t* sctx, hir_ctx_t* hctx, sym_table_t* smt) {
    return HIR_generate_block(sctx->r, hctx, smt);
}

hir_block_t* HIR_get_next(hir_block_t* c, hir_block_t* exit, int skip) {
    if (!skip) return c;
    if (c == exit) return NULL;
    for (; c && c != exit; c = c->next) {
        if ((c == exit) || skip-- == 0) return c;
        if (c->unused) continue;
    }

    return c;
}
