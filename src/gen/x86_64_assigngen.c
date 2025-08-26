#include <generator.h>

int x86_64_generate_assignment(ast_node_t* node, FILE* output, gen_ctx_t* ctx) {
    ast_node_t* left  = node->child;
    ast_node_t* right = left->sibling;
    x86_64_generate_block(right, output, ctx);
    iprintf(output, "mov %s, %s\n", GET_ASMVAR(left), GET_RAW_REG(BASE_BITNESS, RAX));
    return 1;
}
