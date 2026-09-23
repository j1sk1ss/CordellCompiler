#include <ast/astgen/astgen.h>

DEFINE_PARSER(cpl_parse_call_arguments, {
    int* args = (int*)carry;
    ast_node_t* base = AST_create_node_bt(CREATE_SCOPE_TOKEN);
    while (CURRENT_TOKEN && CURRENT_TOKEN->t_type != CLOSE_BRACKET_TOKEN) {
        ast_node_t* arg = cpl_parse_expression(it, ctx, smt, 1);
        PARSER_ASSERT(!arg, base, "Error during the call argument parsing! <arg>!");
        AST_add_node(base, arg);

        if (args) {
            (*args)++;
        }

        if (CURRENT_TOKEN->t_type == COMMA_TOKEN) {
            forward_token(it, 1);
        }
    }
    
    return base;
})
