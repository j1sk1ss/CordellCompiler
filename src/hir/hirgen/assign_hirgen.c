#include <hir/hirgens/hirgens.h>

int HIR_generate_assignment_block(ast_node_t* node, hir_ctx_t* ctx, sym_table_t* smt) {
    ast_node_t* left   = node->child;
    ast_node_t* right  = left->sibling;
    hir_subject_t* src = HIR_generate_elem(right, ctx, smt);
    src = HIR_generate_conv(ctx, HIR_get_tmptype_tkn(left->token, left->token->flags.ptr), src, smt);
    return HIR_generate_store_block(left, src, ctx, smt);
}
