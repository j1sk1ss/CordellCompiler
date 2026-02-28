#include <ast/astgen/astgen.h>

ast_node_t* cpl_parse_poparg(PARSER_ARGS) {
    PARSER_ARGS_USE;
    SAVE_TOKEN_POINT;

    ast_node_t* node = AST_create_node(CURRENT_TOKEN);
    if (!node) {
        PARSE_ERROR("Can't create the base for the '%s' statement!", POPARG_COMMAND);
        RESTORE_TOKEN_POINT;
        return NULL;
    }
    
    forward_token(it, 1);
    return node;
}
