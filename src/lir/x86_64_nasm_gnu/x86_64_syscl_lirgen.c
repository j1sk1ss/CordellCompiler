#include <lir/x86_64_gnu_nasm/x86_64_irgen.h>

int LIR_generate_start_block(ast_node_t* node, lir_gen_t* g, lir_ctx_t* ctx) {
    IR_BLOCK0(ctx, STRT);
    IR_BLOCK1(ctx, PUSH, IR_SUBJ_REG(RBP, 8));
    IR_BLOCK2(ctx, iMOV, IR_SUBJ_REG(RBP, 8), IR_SUBJ_REG(RSP, 8));
    IR_BLOCK2(ctx, iSUB, IR_SUBJ_REG(RSP, 8), IR_SUBJ_CNST(AST_get_max_offset(node)));

    int arg_count = 0;
    for (ast_node_t* t = node->child; t; t = t->sibling) {
        if (VRS_isblock(t->token)) g->blockgen(t, g, ctx);
        else if (VRS_isdecl(t->token) && arg_count < 2) {
            switch (arg_count++) {
                case 0: IR_BLOCK2(ctx, iMOV, IR_SUBJ_REG(RAX, 8), IR_SUBJ_NOFF(8, 8)); break;
                case 1: IR_BLOCK2(ctx, LEA, IR_SUBJ_REG(RAX, 8), IR_SUBJ_NOFF(16, 8)); break;
            }
            
            g->store(t->child, g, ctx);   
        }
    }

    return 1;
}

int LIR_generate_exit_block(ast_node_t* node, lir_gen_t* g, lir_ctx_t* ctx) {
    IR_deallocate_scope_heap(node, ctx);
    if (VRS_instant_movable(node->child->token)) IR_BLOCK2(ctx, iMOV, IR_SUBJ_REG(RDI, 8), IR_SUBJ_VAR(node->child));
    else {
        g->elemegen(node->child, g, ctx);
        IR_BLOCK2(ctx, iMOV, IR_SUBJ_REG(RDI, 8), IR_SUBJ_REG(RAX, 8));
    }

    IR_BLOCK2(ctx, iMOV, IR_SUBJ_REG(RAX, 8), IR_SUBJ_CNST(60));
    IR_BLOCK0(ctx, SYSC);
    return 1;
}

static const int args_regs[] = { RAX, RDI, RSI, RDX, R10, R8, R9 };
int LIR_generate_syscall_block(ast_node_t* node, lir_gen_t* g, lir_ctx_t* ctx) {
    int arg_index = 0;
    ast_node_t* args = node->child;
    while (args && arg_index < 7) {
        g->elemegen(args, g, ctx);
        IR_BLOCK2(ctx, iMOV, IR_SUBJ_REG(args_regs[arg_index], 8), IR_SUBJ_REG(RAX, 8));
        IR_BLOCK1(ctx, PUSH, IR_SUBJ_REG(args_regs[arg_index++], 8));
        args = args->sibling;
    }

    for (int i = arg_index - 1; i >= 0; i--) {
        IR_BLOCK1(ctx, POP, IR_SUBJ_REG(args_regs[i], 8));
    }

    IR_BLOCK0(ctx, SYSC);
    return 1;
}
