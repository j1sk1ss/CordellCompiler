#include <ast/astgen/astgen.h>

ast_node_t* cpl_parse_syscall(list_iter_t* it, ast_ctx_t* ctx, sym_table_t* smt) {
    SAVE_TOKEN_POINT;

    ast_node_t* node = AST_create_node(CURRENT_TOKEN);
    if (!node) {
        PARSE_ERROR("Can't create a base for the syscall statement!");
        RESTORE_TOKEN_POINT;
        return NULL;
    }

    forward_token(it, 1);
    if (CURRENT_TOKEN->t_type != OPEN_BRACKET_TOKEN) {
        PARSE_ERROR("Expected the 'OPEN_BRACKET_TOKEN' token during a parse of the '%s' statement!", SYSCALL_COMMAND);
        AST_unload(node);
        RESTORE_TOKEN_POINT;
        return NULL;
    }

    forward_token(it, 1);
    while (CURRENT_TOKEN && CURRENT_TOKEN->t_type != CLOSE_BRACKET_TOKEN) {
        if (CURRENT_TOKEN->t_type == COMMA_TOKEN) {
            forward_token(it, 1);
            continue;
        }

        ast_node_t* arg = cpl_parse_expression(it, ctx, smt);
        if (arg) AST_add_node(node, arg);
        else { 
            PARSE_ERROR("Error during the syscall's argument parsing! syscall(<statement>)!");
            AST_unload(node);
            RESTORE_TOKEN_POINT;
            return NULL;
        }
    }

    return node;
}
