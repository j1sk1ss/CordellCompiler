#include <ir/x86_64_gnu_nasm/x86_64_irgen.h>

int IR_generate_assignment_block(ast_node_t* node, ir_gen_t* g, ir_ctx_t* ctx) {
    ast_node_t* left  = node->child;
    ast_node_t* right = left->sibling;
    g->elemegen(right, g, ctx);
    g->store(left, g, ctx);
    return 1;
}
