#include <generator.h>

int x86_64_generate_elem(ast_node_t* node, FILE* output, gen_ctx_t* ctx) {
    if (!node) return 0;
    if (VRS_isblock(node->token)) {
        return x86_64_generate_elem(node->child, output, ctx);
    }

    if (VRS_isdecl(node->token)) x86_64_generate_declaration(node, output, ctx);
    if (
        VRS_isoperand(node->token) && 
        node->token->t_type != ASSIGN_TOKEN
    ) x86_64_generate_operand(node, output, ctx);

    switch (node->token->t_type) {
        case IF_TOKEN:      x86_64_generate_if(node, output, ctx);         break;
        case SWITCH_TOKEN:  x86_64_generate_switch(node, output, ctx);     break;
        case WHILE_TOKEN:   x86_64_generate_while(node, output, ctx);      break;
        case CALL_TOKEN:    x86_64_generate_funccall(node, output, ctx);   break;
        case FUNC_TOKEN:    x86_64_generate_function(node, output, ctx);   break;
        case RETURN_TOKEN:  x86_64_generate_return(node, output, ctx);     break;
        case START_TOKEN:   x86_64_generate_start(node, output, ctx);      break;
        case EXIT_TOKEN:    x86_64_generate_exit(node, output, ctx);       break;
        case SYSCALL_TOKEN: x86_64_generate_syscall(node, output, ctx);    break;
        case ASSIGN_TOKEN:  x86_64_generate_assignment(node, output, ctx); break;
        case UNKNOWN_NUMERIC_TOKEN:
        case CHAR_VALUE_TOKEN:
        case LONG_VARIABLE_TOKEN:
        case INT_VARIABLE_TOKEN:
        case SHORT_VARIABLE_TOKEN:
        case CHAR_VARIABLE_TOKEN:
        case ARR_VARIABLE_TOKEN:
        case STR_VARIABLE_TOKEN: x86_64_generate_load(node, output, ctx); break;
    }

    return 1;
}

int x86_64_generate_block(ast_node_t* node, FILE* output, gen_ctx_t* ctx) {
    for (ast_node_t* t = node; t; t = t->sibling) {
        if (VRS_isblock(t->token)) {
            x86_64_generate_block(t->child, output, ctx);
            continue;
        }

        x86_64_generate_elem(t, output, ctx);
    }

    return 1;
}
