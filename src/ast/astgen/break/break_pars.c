#include <ast/astgen/astgen.h>

ast_node_t* cpl_parse_break(PARSER_ARGS) {
    PARSER_ARGS_USE;
    SAVE_TOKEN_POINT;

    ast_node_t* base = AST_create_node(CURRENT_TOKEN);
    if (!base) {
        PARSE_ERROR("Can't create a base for the '%s' statement!", BREAK_COMMAND);
        RESTORE_TOKEN_POINT;
        return NULL;
    }

    if (!consume_token(it, DELIMITER_TOKEN)) {
        PARSE_ERROR("Delimiter token isn't found!");
        AST_unload(base);
        RESTORE_TOKEN_POINT;
        return NULL;
    }
    
    forward_token(it, 1);
    return base;
}
