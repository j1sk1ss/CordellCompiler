#include <generator.h>

static const int args_regs[] = { RDI, RSI, RDX, RCX, R8, R9 };

int x86_64_generate_funccall(ast_node_t* node, FILE* output, gen_ctx_t* ctx) {
    int variables_size = 0;
    ast_node_t* name = node;

    int arg_count = 0;
    ast_node_t* args[128] = { NULL };
    for (ast_node_t* arg = name->child; arg && arg_count < 128; arg = arg->sibling) {
        args[arg_count++] = arg;
    }

    int pushed_args = 0;
    for (pushed_args = 0; pushed_args < MIN(arg_count, 6); pushed_args++) {
        x86_64_generate_block(args[pushed_args], output, ctx);
        iprintf(output, "mov %s, %s \n", GET_RAW_REG(BASE_BITNESS, args_regs[pushed_args]), GET_RAW_REG(BASE_BITNESS, RAX));
    }

    int stack_args = arg_count - pushed_args - 1;
    while (stack_args >= 0) {
        x86_64_generate_block(args[stack_args--], output, ctx);
        iprintf(output, "push %s\n", GET_RAW_REG(BASE_BITNESS, RAX));
    }

    iprintf(output, "call __%s__\n", name->token->value);
    if (stack_args > 0) {
        iprintf(output, "add %s, %d\n", GET_RAW_REG(BASE_BITNESS, RSP), stack_args * (BASE_BITNESS / 8));
    }
}

int x86_64_generate_function(ast_node_t* node, FILE* output, gen_ctx_t* ctx) {
    ast_node_t* name_node   = node->child;
    ast_node_t* params_node = name_node->sibling;
    ast_node_t* body_node   = params_node->sibling;

    /* Entry jumo guard */
    iprintf(output, "jmp __end_%s__\n", name_node->token->value);
    iprintf(output, "__%s__:\n", name_node->token->value);

    /* Stack save */
    iprintf(output, "push %s\n", GET_RAW_REG(BASE_BITNESS, RBP));
    iprintf(output, "mov %s, %s\n", GET_RAW_REG(BASE_BITNESS, RBP), GET_RAW_REG(BASE_BITNESS, RSP));

    /* Function local variables size */
    int lvsize = (
        get_stack_size(params_node->child, name_node->token->value, ctx) + /* arguments size */
        get_stack_size(body_node->child, name_node->token->value, ctx)     /* used variables in function */
    );

    /* Stack reservation */
    iprintf(output, "sub %s, %d\n", GET_RAW_REG(BASE_BITNESS, RSP), ALIGN(lvsize));

    /* Loading input args to stack */
    int pop_params = 0;
    int stack_offset = 8;

    for (ast_node_t* param = params_node->child; param; param = param->sibling) {
        int param_size = param->info.offset;

        regs_t reg;
        get_reg(&reg, VRS_variable_bitness(param->child->token, 1) / 8, args_regs[pop_params], 0);

        if (pop_params++ < 6) {
            iprintf( /* x86_64 argument from register */
                output, "mov%s[%s - %d], %s\n", 
                reg.operation, GET_RAW_REG(BASE_BITNESS, RBP), param->child->info.offset, reg.name
            );
        }
        else { /* argument from stack */
            iprintf(output, "mov %s, [%s + %d]\n", reg.name, GET_RAW_REG(BASE_BITNESS, RBP), stack_offset);
            iprintf(output, "mov%s[%s - %d], %s\n", reg.operation, GET_RAW_REG(BASE_BITNESS, RBP), param->child->info.offset, reg.name);
            stack_offset += param_size;
        }
    }

    x86_64_generate_block(body_node, output, ctx);
    iprintf(output, "__end_%s__:\n", name_node->token->value);
    return 1;
}
