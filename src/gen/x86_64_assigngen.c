#include <generator.h>

int x86_64_generate_assignment(ast_node_t* node, FILE* output, gen_ctx_t* ctx) {
    ast_node_t* left  = node->child;
    ast_node_t* right = left->sibling;
    x86_64_generate_block(right, output, ctx);
    x86_64_generate_load(left, output, ctx);
    return 1;
}
