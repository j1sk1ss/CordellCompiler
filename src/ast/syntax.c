/* LL(1) parser */
#include <syntax.h>

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

int STX_create(token_t* head, syntax_ctx_t* ctx) {
    token_t* curr_head = head;
    ctx->r = ctx->block(&curr_head, ctx, CLOSE_BLOCK_TOKEN);
    return ctx->r != NULL;
}
