#include <lir/x86_64_gnu_nasm/x86_64_irgen.h>

int LIR_generate_funcdef_block(ast_node_t* node, lir_gen_t* g, lir_ctx_t* ctx) {
    if (!node) return 0;
    for (ast_node_t* t = node; t; t = t->sibling) {
        if (VRS_isblock(t->token)) {
            g->funcdef(t->child, g, ctx);
            continue;
        }

        if (
            t->token->t_type == FUNC_TOKEN && t->token->flags.glob
        ) IR_BLOCK1(ctx, FDCL, IR_SUBJ_STR(8, t->child->token->value));
        else if (t->token->t_type == IMPORT_SELECT_TOKEN) {
            for (ast_node_t* func = t->child->child; func; func = func->sibling) {
                IR_BLOCK1(ctx, OEXT, IR_SUBJ_STR(8, func->token->value));
            }
        }
    }

    return 1;
}

int LIR_generate_return_block(ast_node_t* node, lir_gen_t* g, lir_ctx_t* ctx) {
    IR_deallocate_scope_heap(node, ctx);
    if (node->child) g->elemegen(node->child, g, ctx);
    IR_BLOCK2(ctx, iMOV, IR_SUBJ_REG(RSP, 8), IR_SUBJ_REG(RBP, 8));
    IR_BLOCK1(ctx, POP, IR_SUBJ_REG(RBP, 8));
    IR_BLOCK0(ctx, FRET);
    return 1;
}

static const int args_regs[] = { RDI, RSI, RDX, RCX, R8, R9 };
int LIR_generate_funccall_block(ast_node_t* node, lir_gen_t* g, lir_ctx_t* ctx) {
    int variables_size = 0;
    ast_node_t* name = node;

    int arg_count = 0;
    ast_node_t* args[128] = { NULL };
    for (ast_node_t* arg = name->child; arg && arg_count < 128; arg = arg->sibling) {
        args[arg_count++] = arg;
    }

    int pushed_args = 0;
    for (pushed_args = 0; pushed_args < MIN(arg_count, 6); pushed_args++) {
        if (!args[pushed_args]) continue;
        g->elemegen(args[pushed_args], g, ctx);
        IR_BLOCK2(ctx, iMOV, IR_SUBJ_REG(args_regs[pushed_args], 8), IR_SUBJ_REG(RAX, 8));
    }

    int stack_args = arg_count - pushed_args - 1;
    while (stack_args >= 0) {
        g->elemegen(args[stack_args--], g, ctx);
        IR_BLOCK1(ctx, PUSH, IR_SUBJ_REG(RAX, 8));
    }

    IR_BLOCK1(ctx, !name->token->flags.ext ? FCLL : ECLL, IR_SUBJ_STR(8, name->token->value));
    if (stack_args > 0) {
        IR_BLOCK2(ctx, iADD, IR_SUBJ_REG(RSP, 8), IR_SUBJ_CNST(stack_args * (BASE_BITNESS / 8)));
    }

    return 1;
}

int LIR_generate_function_block(ast_node_t* node, lir_gen_t* g, lir_ctx_t* ctx) {
    ast_node_t* name_node = node->child;
    ast_node_t* body_node = name_node->sibling;

    IR_BLOCK1(ctx, JMP, IR_SUBJ_STR(8, "_f%s_e", name_node->token->value));
    IR_BLOCK1(ctx, FDCL, IR_SUBJ_STR(8, name_node->token->value));

    IR_BLOCK1(ctx, PUSH, IR_SUBJ_REG(RBP, 8));
    IR_BLOCK2(ctx, iMOV, IR_SUBJ_REG(RBP, 8), IR_SUBJ_REG(RSP, 8));

    int lvsize = AST_get_max_offset(body_node);
    IR_BLOCK2(ctx, iSUB, IR_SUBJ_REG(RSP, 8), IR_SUBJ_CNST(lvsize));

    int pop_params = 0;
    int stack_offset = 8;

    ast_node_t* t = NULL;
    for (t = body_node->child; t && t->token->t_type != SCOPE_TOKEN; t = t->sibling) {
        int param_size = t->sinfo.offset;
        if (pop_params < 6) {
            IR_BLOCK2(ctx, iMOV, IR_SUBJ_REG(RAX, 8), IR_SUBJ_REG(args_regs[pop_params], 8));
        }
        else {
            IR_BLOCK2(ctx, iMOV, IR_SUBJ_REG(RAX, 8), IR_SUBJ_OFF(stack_offset, 8));
            stack_offset += BASE_BITNESS / 8;
        }

        IR_BLOCK2(
            ctx, iMOV, IR_SUBJ_OFF(t->child->sinfo.offset, VRS_variable_bitness(t->child->token, 1) / 8), 
            IR_SUBJ_REG(args_regs[pop_params++], 8)
        );
    }

    g->blockgen(t, g, ctx);
    IR_BLOCK1(ctx, MKLB, IR_SUBJ_STR(8, "_f%s_e", name_node->token->value));
    return 1;
}
