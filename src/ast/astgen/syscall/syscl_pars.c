#include <ast/astgen/astgen.h>

ast_node_t* cpl_parse_syscall(PARSER_ARGS) {
    PARSER_ARGS_USE;
    SAVE_TOKEN_POINT;

    ast_node_t* base = AST_create_node(CURRENT_TOKEN);
    PARSER_DO_OR_THROW(!base, NULL, "Can't create a base for the syscall statement!");
    PARSER_DO_OR_THROW(
        !consume_token(it, OPEN_BRACKET_TOKEN), base, 
        "Expected the 'OPEN_BRACKET_TOKEN' token during a parse of the 'syscall' statement!"
    );

    forward_token(it, 1);
    while (CURRENT_TOKEN && CURRENT_TOKEN->t_type != CLOSE_BRACKET_TOKEN) {
        if (CURRENT_TOKEN->t_type == COMMA_TOKEN) {
            forward_token(it, 1);
            continue;
        }

        ast_node_t* arg = cpl_parse_expression(it, ctx, smt, 1);
        PARSER_DO_OR_THROW(!arg, base, "Error during the syscall's argument parsing! syscall(<statement>)!");
        AST_add_node(base, arg);
    }

    forward_token(it, 1);
    return base;
}
