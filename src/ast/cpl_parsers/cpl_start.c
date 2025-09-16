#include <cpl_parser.h>

ast_node_t* cpl_parse_start(token_t** curr, syntax_ctx_t* ctx, parser_t* p) {
    ast_node_t* node = AST_create_node(*curr);
    if (!node) return NULL;

    forward_token(curr, 1);
    while (*curr && (*curr)->t_type != CLOSE_BRACKET_TOKEN) {
        if ((*curr)->t_type == COMMA_TOKEN) {
            forward_token(curr, 1);
            continue;
        }

        if (!VRS_isdecl((*curr))) forward_token(curr, 1);
        else {
            ast_node_t* arg = p->vardecl(curr, ctx, p);
            if (!arg) {
                print_error("AST error during function arg parsing! line=%i", (*curr)->lnum);
                AST_unload(node);
                return NULL;
            }

            AST_add_node(node, arg);
        }
    }

    forward_token(curr, 1);
    ast_node_t* body = p->block(curr, ctx, TOKEN_MASK(CLOSE_BLOCK_TOKEN), p);
    AST_add_node(node, body);

    return node;
}
