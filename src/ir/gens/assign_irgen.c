#include <gen/x86_64_nasm_gnu/x86_64_gnu_nasm.h>

int x86_64_generate_assignment(ast_node_t* node, FILE* output, gen_ctx_t* ctx, gen_t* g) {
    ast_node_t* left  = node->child;
    ast_node_t* right = left->sibling;

    if (
        VRS_one_slot(left->token) && !left->child && /* Left is one slot and without indexing */
        (VRS_isnumeric(right->token))                /* Right is numeric */
    ) {
        regs_t reg;
        get_reg(&reg, VRS_variable_bitness(left->token, 1) / 8, RAX, 0);
        iprintf(output, "mov%s%s, %s\n", reg.operation, GET_ASMVAR(left), GET_ASMVAR(right));
    }
    else {
        g->elemegen(right, output, ctx, g);
        g->store(left, output, ctx, g);
    }

    return 1;
}
