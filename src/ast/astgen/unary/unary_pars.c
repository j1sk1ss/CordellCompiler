#include <ast/astgen/astgen.h>

DEFINE_PARSER(cpl_parse_unary, {
    ast_node_t* base = AST_create_node(CURRENT_TOKEN);
    PARSER_ASSERT(!base, NULL, "Can't create a base for an unary command!");

    forward_token(it, 1);
    ast_node_t* body = cpl_parse_expression(it, ctx, smt, 2);
    PARSER_ASSERT(!body, base, "Error during the unary's body parse! <unary> <exp>!");
    AST_add_node(base, body);
    return base;
})
