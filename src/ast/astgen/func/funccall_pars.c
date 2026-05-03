#include <ast/astgen/astgen.h>

ast_node_t* cpl_parse_call_arguments(PARSER_ARGS) {
    PARSER_ARGS_USE;
    SAVE_TOKEN_POINT;

    int* args = (int*)carry;
    ast_node_t* base = AST_create_node_bt(CREATE_SCOPE_TOKEN);
    while (CURRENT_TOKEN && CURRENT_TOKEN->t_type != CLOSE_BRACKET_TOKEN) {
        ast_node_t* arg = cpl_parse_expression(it, ctx, smt, 1);
        if (arg) AST_add_node(base, arg);
        else { 
            PARSE_ERROR("Error during the call argument parsing! <arg>!");
            AST_unload(base);
            RESTORE_TOKEN_POINT;
            return NULL; 
        }

        if (args) {
            (*args)++;
        }

        if (CURRENT_TOKEN->t_type == COMMA_TOKEN) {
            forward_token(it, 1);
        }
    } 
    return base;
}
