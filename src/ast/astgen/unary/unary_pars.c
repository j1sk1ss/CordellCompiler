#include <ast/astgen/astgen.h>

ast_node_t* cpl_parse_unary(PARSER_ARGS) {
    PARSER_ARGS_USE;
    SAVE_TOKEN_POINT;

    ast_node_t* base = AST_create_node(CURRENT_TOKEN);
    if (!base) {
        PARSE_ERROR("Can't create a base for an unary command!");
        RESTORE_TOKEN_POINT;
        return NULL;
    }

    forward_token(it, 1);
    ast_node_t* body = cpl_parse_expression(it, ctx, smt, 2);
    if (body) AST_add_node(base, body);
    else {
        PARSE_ERROR("Error during the unary's body parse! <unary> <exp>!");
        AST_unload(base);
        RESTORE_TOKEN_POINT;
        return NULL;
    }

    return base;
}
