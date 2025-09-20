#include <ast/parsers/parser.h>

ast_node_t* cpl_parse_syscall(token_t** curr, syntax_ctx_t* ctx) {
    ast_node_t* node = AST_create_node(*curr);
    if (!node) return NULL;

    forward_token(curr, 1);
    if ((*curr)->t_type == OPEN_BRACKET_TOKEN) {
        forward_token(curr, 1);
        while (*curr && (*curr)->t_type != CLOSE_BRACKET_TOKEN) {
            if ((*curr)->t_type == COMMA_TOKEN) {
                forward_token(curr, 1);
                continue;
            }

            ast_node_t* arg = cpl_parse_expression(curr, ctx);
            if (arg) AST_add_node(node, arg);
        }
    }

    return node;
}
