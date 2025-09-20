#include <ir/hir/hir.h>

ir_subject_t* HIR_generate_if_block(ast_node_t* node, ir_ctx_t* ctx) {
    int current_label   = ctx->lid++;
    ast_node_t* cond    = node->child;
    ast_node_t* lbranch = cond->sibling;
    ast_node_t* rbranch = lbranch->sibling;
    return 1;
}

ir_subject_t* HIR_generate_while_block(ast_node_t* node, ir_ctx_t* ctx) {
    int current_label   = ctx->lid++;
    ast_node_t* cond    = node->child;
    ast_node_t* lbranch = cond->sibling;
    ast_node_t* rbranch = lbranch->sibling;
    return 1;
}

ir_subject_t* HIR_generate_switch_block(ast_node_t* node, ir_ctx_t* ctx) {
    int current_label = ctx->lid++;
    ast_node_t* cond  = node->child;
    ast_node_t* cases = cond->sibling;
    return 1;
}
