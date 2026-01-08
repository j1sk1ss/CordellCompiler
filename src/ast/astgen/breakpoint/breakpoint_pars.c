/* The 'lis' keyword parser
   - 'lis' keyword */
#include <ast/astgen/astgen.h>

ast_node_t* cpl_parse_breakpoint(list_iter_t* it, ast_ctx_t* ctx, sym_table_t* smt) {
    SAVE_TOKEN_POINT;

    ast_node_t* node = AST_create_node(CURRENT_TOKEN);
    if (!node) {
        PARSE_ERROR("Can't create a base for the '%s' statement!", BREAKPOINT_COMMAND);
        RESTORE_TOKEN_POINT;
        return NULL;
    }

    forward_token(it, 1);
    if (CURRENT_TOKEN->t_type == STRING_VALUE_TOKEN) {
        ast_node_t* info = cpl_parse_expression(it, ctx, smt, 1);
        if (!node) {
            PARSE_ERROR("Can't create a base for the '%s' statement message!", BREAKPOINT_COMMAND);
            RESTORE_TOKEN_POINT;
            return NULL;
        }

        AST_add_node(node, info);
        forward_token(it, 1);
    }

    return node;
}
