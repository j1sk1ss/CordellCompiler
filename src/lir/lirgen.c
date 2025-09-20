#include <lir/irgen.h>

int IR_generate(lir_gen_t* g, lir_ctx_t* ctx) {
    scope_reset(&ctx->heap);
    g->funcdef(ctx->synt->r, g, ctx);
    g->blockgen(ctx->synt->r, g, ctx);
    return 1;
}
