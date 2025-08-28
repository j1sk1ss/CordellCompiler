#include <x86_64_gnu_nasm.h>

int x86_64_generate_assignment(ast_node_t* node, FILE* output, gen_ctx_t* ctx) {
    ast_node_t* left  = node->child;
    ast_node_t* right = left->sibling;
    ctx->blockgen(right, output, ctx);
    ctx->store(left, output, ctx);
    return 1;
}
