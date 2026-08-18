#include <ast/astgen/astgen.h>

ast_node_t* cpl_parse_unary(PARSER_ARGS) {
    PARSER_ARGS_USE;
    SAVE_TOKEN_POINT;

    ast_node_t* base = AST_create_node(CURRENT_TOKEN);
    PARSER_DO_OR_THROW(!base, NULL, "Can't create a base for an unary command!");

    forward_token(it, 1);
    ast_node_t* body = cpl_parse_expression(it, ctx, smt, 2);
    PARSER_DO_OR_THROW(!body, base, "Error during the unary's body parse! <unary> <exp>!");
    AST_add_node(base, body);
    return base;
}
