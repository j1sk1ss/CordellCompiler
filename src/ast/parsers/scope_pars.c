#include <ast/parsers/parser.h>

ast_node_t* cpl_parse_scope(token_t** curr, syntax_ctx_t* ctx, sym_table_t* smt) {
    ast_node_t* node = NULL;
    if ((*curr)->t_type == OPEN_BLOCK_TOKEN) {
        forward_token(curr, 1);
        scope_push_id(&ctx->scopes.stack, ++ctx->scopes.s_id);
        node = cpl_parse_block(curr, ctx, smt, CLOSE_BLOCK_TOKEN);
        if (node) {
            node->token = TKN_create_token(SCOPE_TOKEN, NULL, 0, 0);
            node->sinfo.s_id = scope_id_top(&ctx->scopes.stack);
            forward_token(curr, 1);
        }
    }

    scope_elem_t el;
    scope_pop_top(&ctx->scopes.stack, &el);
    // ctx->symtb.v->offset = el.offset;

    return node;
}
