#include <cpl_parser.h>

ast_node_t* cpl_parse_start(token_t** curr, syntax_ctx_t* ctx, parser_t* p) {
    ast_node_t* node = AST_create_node(*curr);
    if (!node) return NULL;
    forward_token(curr, 2); /* skip start and open block */

    ast_node_t* body = p->block(curr, ctx, CLOSE_BLOCK_TOKEN, p);
    AST_add_node(node, body);

    return node;
}
