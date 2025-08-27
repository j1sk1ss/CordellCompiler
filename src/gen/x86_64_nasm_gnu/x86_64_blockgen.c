#include <generator.h>

int x86_64_generate_block(ast_node_t* node, FILE* output, gen_ctx_t* ctx) {
    for (ast_node_t* t = node; t; t = t->sibling) {
        if (!t->token || t->token->t_type == SCOPE_TOKEN) {
            x86_64_generate_block(t->child, output, ctx);
            continue;
        }

        if (VRS_isdecl(t->token)) x86_64_generate_declaration(t, output, ctx);
        if (
            VRS_isoperand(t->token) && 
            t->token->t_type != ASSIGN_TOKEN
        ) x86_64_generate_operand(t, output, ctx);

        switch (t->token->t_type) {
            case IF_TOKEN:      x86_64_generate_if(t, output, ctx);         break;
            case SWITCH_TOKEN:  x86_64_generate_switch(t, output, ctx);     break;
            case WHILE_TOKEN:   x86_64_generate_while(t, output, ctx);      break;
            case CALL_TOKEN:    x86_64_generate_funccall(t, output, ctx);   break;
            case FUNC_TOKEN:    x86_64_generate_function(t, output, ctx);   break;
            case RETURN_TOKEN:  x86_64_generate_return(t, output, ctx);     break;
            case START_TOKEN:   x86_64_generate_start(t, output, ctx);      break;
            case EXIT_TOKEN:    x86_64_generate_exit(t, output, ctx);       break;
            case SYSCALL_TOKEN: x86_64_generate_syscall(t, output, ctx);    break;
            case ASSIGN_TOKEN:  x86_64_generate_assignment(t, output, ctx); break;
            case LONG_VARIABLE_TOKEN:
            case INT_VARIABLE_TOKEN:
            case SHORT_VARIABLE_TOKEN:
            case CHAR_VARIABLE_TOKEN:
            case ARR_VARIABLE_TOKEN:
            case STR_VARIABLE_TOKEN: x86_64_generate_load(t, output, ctx); break;
        }  
    }
    return 1;
}
