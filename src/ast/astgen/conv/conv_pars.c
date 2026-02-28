#include <ast/astgen/astgen.h>

ast_node_t* cpl_parse_conv(PARSER_ARGS) {
    PARSER_ARGS_USE;
    SAVE_TOKEN_POINT;

    ast_node_t* base = AST_create_node(CURRENT_TOKEN);
    if (!base) {
        PARSE_ERROR("Can't create a base for a cast!");
        RESTORE_TOKEN_POINT;
        return NULL;
    }

    forward_token(it, 1);
    if (CURRENT_TOKEN && !TKN_is_decl(CURRENT_TOKEN)) {
        PARSE_ERROR("Expected a type of a token in a cast!");
        AST_unload(base);
        RESTORE_TOKEN_POINT;
        return NULL;
    }

    ast_node_t* type_node = AST_create_node(CURRENT_TOKEN);
    if (type_node) AST_add_node(base, type_node);
    else {
        PARSE_ERROR("Can't create a base for a type in a cast!");
        AST_unload(base);
        RESTORE_TOKEN_POINT;
        return NULL;
    }

    forward_token(it, 1);
    return base;
}
