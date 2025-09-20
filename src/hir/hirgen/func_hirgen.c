#include <lir/hir/hir.h>

lir_subject_t* HIR_generate_return_block(ast_node_t* node, lir_ctx_t* ctx) {
    // node->child
    return 1;
}

lir_subject_t* HIR_generate_funccall_block(ast_node_t* node, lir_ctx_t* ctx) {
    int variables_size = 0;
    ast_node_t* name = node;

    int arg_count = 0;
    ast_node_t* args[128] = { NULL };
    for (ast_node_t* arg = name->child; arg && arg_count < 128; arg = arg->sibling) {
        args[arg_count++] = arg;
    }

    return 1;
}

lir_subject_t* HIR_generate_function_block(ast_node_t* node, lir_ctx_t* ctx) {
    ast_node_t* name_node = node->child;
    ast_node_t* body_node = name_node->sibling;
    return 1;
}
