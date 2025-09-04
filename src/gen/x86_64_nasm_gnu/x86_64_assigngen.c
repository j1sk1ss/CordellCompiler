#include <x86_64_gnu_nasm.h>

int x86_64_generate_assignment(ast_node_t* node, FILE* output, gen_ctx_t* ctx, gen_t* g) {
    ast_node_t* left  = node->child;
    ast_node_t* right = left->sibling;
    g->elemegen(right, output, ctx, g);
    g->store(left, output, ctx, g);
    return 1;
}
