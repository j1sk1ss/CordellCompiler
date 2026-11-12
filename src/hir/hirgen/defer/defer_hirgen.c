#include <hir/hirgens/hirgens.h>

int HIR_generate_defer_block(ast_node_t* node, hir_ctx_t* ctx, sym_table_t* smt) {
    HIR_SET_CURRENT_POS(ctx, node);
    HIR_BLOCK0(ctx, HIR_DEFER_START);
    HIR_generate_block(node->c, ctx, smt);
    HIR_BLOCK0(ctx, HIR_DEFER_END);
    return 1;
}