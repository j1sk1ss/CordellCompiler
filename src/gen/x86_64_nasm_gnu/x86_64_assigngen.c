#include <x86_64_gnu_nasm.h>

int x86_64_generate_assignment(ast_node_t* node, FILE* output, gen_ctx_t* ctx, gen_t* g) {
    ast_node_t* left  = node->child;
    ast_node_t* right = left->sibling;

    if (
        VRS_one_slot(left->token) && !left->child && /* Left is one slot and without indexing */
        (VRS_isnumeric(right->token))                /* Right is numeric */
    ) iprintf(output, "mov %s, %s\n", GET_ASMVAR(left), GET_ASMVAR(right));
    else {
        g->elemegen(right, output, ctx, g);
        g->store(left, output, ctx, g);
    }

    return 1;
}
