/* The 'lis' keyword parser
   - 'lis' keyword */
#include <ast/astgen/astgen.h>

ast_node_t* cpl_parse_breakpoint(list_iter_t* it) {
    SAVE_TOKEN_POINT;

    ast_node_t* node = AST_create_node(CURRENT_TOKEN);
    if (!node) {
        PARSE_ERROR("Can't create a base for the '%s' statement!", BREAKPOINT_COMMAND);
        RESTORE_TOKEN_POINT;
        return NULL;
    }

    forward_token(it, 1);
    return node;
}
