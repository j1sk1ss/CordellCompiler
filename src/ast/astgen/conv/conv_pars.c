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
    symbol_id_t type = type_lookup(CURRENT_TOKEN, ctx, smt);
    if (CURRENT_TOKEN && (!TKN_is_builtin_type(CURRENT_TOKEN) && type == NO_SYMBOL_ID)) {
        PARSE_ERROR("Expected a type of a token in a cast!");
        AST_unload(base);
        RESTORE_TOKEN_POINT;
        return NULL;
    }

    ast_node_t* type_node = AST_create_node(CURRENT_TOKEN);
    if (type_node) {
        AST_add_node(base, type_node);
        if (type != NO_SYMBOL_ID) {
            type_node->sinfo.t_id = type;
            type_node->t->t_type  = EXTRACT_TYPE_TYPE(type, smt);
        }
    }
    else {
        PARSE_ERROR("Can't create a base for a type in a cast!");
        AST_unload(base);
        RESTORE_TOKEN_POINT;
        return NULL;
    }

    forward_token(it, 1);
    return base;
}
