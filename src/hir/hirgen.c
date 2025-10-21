#include <hir/hirgen.h>

int HIR_generate(syntax_ctx_t* sctx, hir_ctx_t* hctx, sym_table_t* smt) {
    return HIR_generate_block(sctx->r, hctx, smt);
}
