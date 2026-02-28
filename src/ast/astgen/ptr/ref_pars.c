#include <ast/astgen/astgen.h>

ast_node_t* cpl_parse_ref(PARSER_ARGS) {
    PARSER_ARGS_USE;
    SAVE_TOKEN_POINT;

    ast_node_t* base = AST_create_node(CURRENT_TOKEN);
    if (!base) {
        PARSE_ERROR("Can't create a base for the referense command!");
        RESTORE_TOKEN_POINT;
        return NULL;
    }

    forward_token(it, 1);
    ast_node_t* body = cpl_parse_expression(it, ctx, smt, 1);
    if (!body) AST_add_node(base, body);
    else {
        PARSE_ERROR("Error during the reference body parse! ref <exp>!");
        AST_unload(base);
        RESTORE_TOKEN_POINT;
        return NULL;
    }

    return base;
}
