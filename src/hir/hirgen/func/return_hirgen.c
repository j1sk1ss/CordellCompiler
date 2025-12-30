#include <hir/hirgens/hirgens.h>

int HIR_generate_return_block(ast_node_t* node, hir_ctx_t* ctx, sym_table_t* smt) {
    return HIR_BLOCK1(ctx, HIR_FRET, HIR_generate_elem(node->c, ctx, smt));
}
