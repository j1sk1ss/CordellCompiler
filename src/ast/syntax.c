#include <ast/syntax.h>

int STX_var_update(ast_node_t* node, syntax_ctx_t* ctx, const char* name, int size, token_flags_t* flags) {
    if (!node) return 0;
    node->sinfo.size   = size;
    node->sinfo.offset = VRT_add_info(name, size, scope_id_top(&ctx->scopes.stack), flags, ctx->symtb.vars);
    return 1;
}

int STX_var_lookup(ast_node_t* node, syntax_ctx_t* ctx) {
    if (!node) return 0;
    STX_var_lookup(node->sibling, ctx);
    STX_var_lookup(node->child, ctx);

    if (!node->token) return 0;
    variable_info_t varinfo = { .offset = -1 };
    for (int s = ctx->scopes.stack.top; s >= 0; s--) {
        int s_id = ctx->scopes.stack.data[s].id;
        if (VRT_get_info(node->token->value, s_id, &varinfo, ctx->symtb.vars)) {
            break;
        }
    }
    
    if (varinfo.offset == -1) return 0;
    node->sinfo.offset      = varinfo.offset;
    node->sinfo.size        = varinfo.size;
    node->sinfo.s_id        = varinfo.scope;
    node->token->flags.heap = varinfo.heap;
    return 1;
}

int STX_create(token_t* head, syntax_ctx_t* ctx) {
    token_t* curr_head = head;
    ctx->r = cpl_parse_block(&curr_head, ctx, CLOSE_BLOCK_TOKEN);
    return ctx->r != NULL;
}
