#include <cpl_parser.h>

ast_node_t* cpl_parse_asm(token_t** curr, syntax_ctx_t* ctx, parser_t* p) {
    ast_node_t* node = AST_create_node(*curr);
    if (!node) return NULL;
    
    forward_token(curr, 1);
    if (*curr && (*curr)->t_type == OPEN_BRACKET_TOKEN) {
        forward_token(curr, 1);
        while (*curr && (*curr)->t_type != CLOSE_BRACKET_TOKEN) {
            if ((*curr)->t_type == COMMA_TOKEN) {
                forward_token(curr, 1);
                continue;
            }

            ast_node_t* arg = p->expr(curr, ctx, p);
            if (arg) AST_add_node(node, arg);
        }
    }

    ast_node_t* body = AST_create_node(NULL);

    forward_token(curr, 1);
    if (*curr && (*curr)->t_type == OPEN_BLOCK_TOKEN) {
        forward_token(curr, 1);
        while (*curr && (*curr)->t_type != CLOSE_BLOCK_TOKEN) {
            if ((*curr)->t_type == COMMA_TOKEN) {
                forward_token(curr, 1);
                continue;
            }

            ast_node_t* arg = AST_create_node(*curr);
            if (arg) AST_add_node(body, arg);
            forward_token(curr, 1);
        }
    }

    forward_token(curr, 1);
    AST_add_node(node, body);
    return node;
}
