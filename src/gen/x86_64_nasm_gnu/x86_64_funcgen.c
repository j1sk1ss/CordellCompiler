#include <x86_64_gnu_nasm.h>

int x86_64_generate_funcdef(ast_node_t* node, FILE* output, gen_ctx_t* ctx, gen_t* g) {
    if (!node) return 0;
    for (ast_node_t* t = node; t; t = t->sibling) {
        if (VRS_isblock(t->token)) {
            g->funcdef(t->child, output, ctx, g);
            continue;
        }

        if (t->token->t_type == FUNC_TOKEN && t->token->vinfo.glob) iprintf(output, "global __%s__\n", t->child->token->value);
        else if (t->token->t_type == IMPORT_SELECT_TOKEN) {
            for (ast_node_t* func = t->child->child; func; func = func->sibling) {
                iprintf(output, "extern __%s__\n", func->token->value);
            }
        }
    }

    return 1;
}

int x86_64_generate_return(ast_node_t* node, FILE* output, gen_ctx_t* ctx, gen_t* g) {
    if (node->child) g->elemegen(node->child, output, ctx, g);
    iprintf(output, "mov rsp, rbp\n");
    iprintf(output, "pop rbp\n");
    iprintf(output, "ret\n");
    return 1;
}

static const int args_regs[] = { RDI, RSI, RDX, RCX, R8, R9 };

int x86_64_generate_funccall(ast_node_t* node, FILE* output, gen_ctx_t* ctx, gen_t* g) {
    int variables_size = 0;
    ast_node_t* name = node;

    int arg_count = 0;
    ast_node_t* args[128] = { NULL };
    for (ast_node_t* arg = name->child; arg && arg_count < 128; arg = arg->sibling) {
        args[arg_count++] = arg;
    }

    int pushed_args = 0;
    for (pushed_args = 0; pushed_args < MIN(arg_count, 6); pushed_args++) {
        g->elemegen(args[pushed_args], output, ctx, g);
        iprintf(output, "mov %s, %s \n", GET_RAW_REG(BASE_BITNESS, args_regs[pushed_args]), GET_RAW_REG(BASE_BITNESS, RAX));
    }

    int stack_args = arg_count - pushed_args - 1;
    while (stack_args >= 0) {
        g->elemegen(args[stack_args--], output, ctx, g);
        iprintf(output, "push %s\n", GET_RAW_REG(BASE_BITNESS, RAX));
    }

    if (!name->token->vinfo.ext) iprintf(output, "call __%s__\n", name->token->value);
    else iprintf(output, "call %s\n", name->token->value);
    if (stack_args > 0) {
        iprintf(output, "add rsp, %d\n", stack_args * (BASE_BITNESS / 8));
    }
}

int x86_64_generate_function(ast_node_t* node, FILE* output, gen_ctx_t* ctx, gen_t* g) {
    ast_node_t* name_node = node->child;
    ast_node_t* body_node = name_node->sibling;

    /* Entry jumo guard */
    iprintf(output, "jmp __end_%s__\n", name_node->token->value);
    iprintf(output, "__%s__:\n", name_node->token->value);

    /* Stack save */
    iprintf(output, "push rbp\n");
    iprintf(output, "mov rbp, rsp\n");

    /* Function local variables size */
    int lvsize = get_stack_size(body_node, ctx);
    iprintf(output, "sub rsp, %d\n", ALIGN(lvsize));

    /* Loading input args to stack */
    int pop_params = 0;
    int stack_offset = 8;

    ast_node_t* t = NULL;
    for (t = body_node->child; t && t->token->t_type != SCOPE_TOKEN; t = t->sibling) {
        int param_size = t->info.offset;
        if (pop_params < 6) {
            regs_t reg;
            get_reg(&reg, VRS_variable_bitness(t->child->token, 1) / 8, args_regs[pop_params], 0);
            iprintf(output, "mov%s[rbp - %d], %s\n", reg.operation, t->child->info.offset, reg.name);
        }
        else { /* argument from stack */
            iprintf(output, "mov rax, [rbp + %d]\n", stack_offset);
            iprintf(output, "mov qword [rbp - %d], rax\n", t->child->info.offset);
            stack_offset += BASE_BITNESS / 8;
        }

        pop_params++;
    }

    g->blockgen(t, output, ctx, g);
    iprintf(output, "__end_%s__:\n", name_node->token->value);
    return 1;
}
