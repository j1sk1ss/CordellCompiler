/* LL(1) parser */
#include <syntax.h>

int STX_create(token_t* head, syntax_ctx_t* ctx) {
    token_t* curr_head = head;
    ctx->r = ctx->block(&curr_head, ctx, CLOSE_BLOCK_TOKEN);
    return ctx->r != NULL;
}
