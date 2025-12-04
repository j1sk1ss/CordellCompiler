#include <ast/astgens/astgens.h>

ast_node_t* cpl_parse_scope(list_iter_t* it, ast_ctx_t* ctx, sym_table_t* smt) {
    ast_node_t* node = NULL;
    if (CURRENT_TOKEN->t_type == OPEN_BLOCK_TOKEN) {
        forward_token(it, 1);
        scope_push_id(&ctx->scopes.stack, ++ctx->scopes.s_id);
        node = cpl_parse_block(it, ctx, smt, CLOSE_BLOCK_TOKEN);
        if (node) {
            node->sinfo.s_id = scope_id_top(&ctx->scopes.stack);
            forward_token(it, 1);
        }
    }

    scope_elem_t el;
    scope_pop_top(&ctx->scopes.stack, &el);
    return node;
}
