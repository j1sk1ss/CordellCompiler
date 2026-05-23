#include <ast/astgen/astgen.h>

ast_node_t* cpl_parse_contdot(PARSER_ARGS) {
    PARSER_ARGS_USE;
    SAVE_TOKEN_POINT;

    ast_node_t* base = AST_create_node(CURRENT_TOKEN);
    if (!base) {
        PARSE_ERROR("Can't create a base for a container!");
        RESTORE_TOKEN_POINT;
        return NULL;
    }

    return base; // TODO:
}