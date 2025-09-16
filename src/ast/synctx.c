#include <synctx.h>

syntax_ctx_t* STX_create_ctx() {
    syntax_ctx_t* ctx = (syntax_ctx_t*)mm_malloc(sizeof(syntax_ctx_t));
    if (!ctx) return NULL;
    str_memset(ctx, 0, sizeof(syntax_ctx_t));
    return ctx;
}

int STX_destroy_ctx(syntax_ctx_t* ctx) {
    if (!ctx) return 0;
    mm_free(ctx);
    return 1;
}

int STX_var_update(ast_node_t* node, syntax_ctx_t* ctx, const char* name, int size, char ro, char glob, int heap) {
    if (!node) return 0;
    node->info.size   = size;
    node->info.offset = VRT_add_info(name, size, ro, glob, scope_id_top(&ctx->scopes.stack), heap, ctx->symtb.vars);
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
    node->info.offset       = varinfo.offset;
    node->info.size         = varinfo.size;
    node->info.s_id         = varinfo.scope;
    node->token->vinfo.heap = varinfo.heap;
    return 1;
}
