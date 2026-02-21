#include <ast/astgen/astgen.h>

ast_node_t* cpl_parse_conv(list_iter_t* it) {
    SAVE_TOKEN_POINT;

    ast_node_t* node = AST_create_node(CURRENT_TOKEN);
    if (!node) {
        PARSE_ERROR("Can't create a base for a cast!");
        RESTORE_TOKEN_POINT;
        return NULL;
    }

    forward_token(it, 1);
    if (CURRENT_TOKEN && !TKN_isdecl(CURRENT_TOKEN)) {
        PARSE_ERROR("Expected a type of a token in a cast!");
        AST_unload(node);
        RESTORE_TOKEN_POINT;
        return NULL;
    }

    ast_node_t* type_node = AST_create_node(CURRENT_TOKEN);
    if (!type_node) {
        PARSE_ERROR("Can't create a base for a type in a cast!");
        AST_unload(node);
        RESTORE_TOKEN_POINT;
        return NULL;
    }

    AST_add_node(node, type_node);
    forward_token(it, 1);
    return node;
}
