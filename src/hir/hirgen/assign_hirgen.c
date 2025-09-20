#include <hir/hirgen/hirgen.h>

int HIR_generate_assignment_block(ast_node_t* node, hir_ctx_t* ctx) {
    ast_node_t* left  = node->child;
    ast_node_t* right = left->sibling;
    HIR_BLOCK2(
        ctx, LOADOP, 
        HIR_SUBJ_VAR(left->sinfo.offset, left->sinfo.size, HIR_get_stktype(left->token)), 
        HIR_generate_elem_block(right, ctx)
    );

    return 1;
}
