#include <cpl_parser.h>

ast_node_t* cpl_parse_start(token_t** curr, syntax_ctx_t* ctx) {
    ast_node_t* node = AST_create_node(*curr);
    if (!node) return NULL;
    forward_token(curr, 2); /* skip start and open block */

    ast_node_t* body = ctx->block(curr, ctx, CLOSE_BLOCK_TOKEN);
    AST_add_node(node, body);

    return node;
}
