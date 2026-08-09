#include <ast/astgen/astgen.h>

ast_node_t* cpl_parse_breakpoint(PARSER_ARGS) {
    PARSER_ARGS_USE;
    SAVE_TOKEN_POINT;

    ast_node_t* node = AST_create_node(CURRENT_TOKEN);
    if (!node) {
        PARSE_ERROR("Can't create a base for a 'lis' statement!");
        RESTORE_TOKEN_POINT;
        return NULL;
    }

    if (consume_token(it, STRING_VALUE_TOKEN)) {
        ast_node_t* info = AST_create_node(CURRENT_TOKEN);
        if (!info) {
            PARSE_ERROR("Can't create a base for a 'lis' message!");
            AST_unload(node);
            RESTORE_TOKEN_POINT;
            return NULL;
        }

        if ((info->sinfo.v_id = STTB_add_info(info->t->body, STR_COMMENT, &smt->s)) == NO_SYMBOL_ID) {
            PARSE_ERROR("Can't register '%s' for a 'lis' statement!", info->t->body->body);
            AST_unload(node);
            RESTORE_TOKEN_POINT;
            return NULL;
        }

        AST_add_node(node, info);
        forward_token(it, 1);
    }

    forward_token(it, 1);
    return node;
}
