#include <ast/parsers/parser.h>

ast_node_t* cpl_parse_syscall(list_iter_t* it, syntax_ctx_t* ctx, sym_table_t* smt) {
    ast_node_t* node = AST_create_node((token_t*)list_iter_current(it));
    if (!node) return NULL;

    forward_token(it, 1);
    if (((token_t*)list_iter_current(it))->t_type == OPEN_BRACKET_TOKEN) {
        forward_token(it, 1);
        while ((token_t*)list_iter_current(it) && ((token_t*)list_iter_current(it))->t_type != CLOSE_BRACKET_TOKEN) {
            if (((token_t*)list_iter_current(it))->t_type == COMMA_TOKEN) {
                forward_token(it, 1);
                continue;
            }

            ast_node_t* arg = cpl_parse_expression(it, ctx, smt);
            if (arg) AST_add_node(node, arg);
        }
    }

    return node;
}
