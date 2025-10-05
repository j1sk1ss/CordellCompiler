#include <ast/syntax.h>

int STX_create(list_t* tkn, syntax_ctx_t* ctx, sym_table_t* smt) {
    list_iter_t it;
    list_iter_hinit(tkn, &it);
    ctx->r = cpl_parse_block(&it, ctx, smt, CLOSE_BLOCK_TOKEN);
    return ctx->r != NULL;
}
