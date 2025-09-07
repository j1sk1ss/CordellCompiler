#include <x86_64_gnu_nasm.h>

int x86_64_generate_assignment(ast_node_t* node, FILE* output, gen_ctx_t* ctx, gen_t* g) {
    ast_node_t* left  = node->child;
    ast_node_t* right = left->sibling;

    if (
        (right->token->t_type == UNKNOWN_NUMERIC_TOKEN ||
        right->token->t_type == UNKNOWN_CHAR_TOKEN) &&
        VRS_one_slot(left->token) && !left->child /* One slot and without indexing */
    ) {
        if (right->token->t_type == UNKNOWN_CHAR_TOKEN) {
            iprintf(output, "mov %s, %i\n", GET_ASMVAR(left), right->token->value[0]);
        }
        else {
            iprintf(output, "mov %s, %s\n", GET_ASMVAR(left), right->token->value);
        }
    }
    else {
        g->elemegen(right, output, ctx, g);
        g->store(left, output, ctx, g);
    }

    return 1;
}
