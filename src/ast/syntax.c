/* LL(1) parser */
#include <syntax.h>

int STX_create(token_t* head, syntax_ctx_t* ctx, parser_t* p) {
    token_t* curr_head = head;
    ctx->r = p->block(&curr_head, ctx, CLOSE_BLOCK_TOKEN, p);
    return ctx->r != NULL;
}
