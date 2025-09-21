#include <ast/syntax.h>

int STX_create(token_t* head, syntax_ctx_t* ctx, sym_table_t* smt) {
    token_t* curr_head = head;
    ctx->r = cpl_parse_block(&curr_head, ctx, smt, CLOSE_BLOCK_TOKEN);
    return ctx->r != NULL;
}
