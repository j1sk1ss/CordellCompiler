#include <ir/irgen.h>

int IR_generate(ir_gen_t* g, ir_ctx_t* ctx) {
    g->funcdef(ctx->synt->r, g, ctx);
    scope_reset(&ctx->heap);
    g->blockgen(ctx->synt->r, g, ctx);
    return 1;
}
