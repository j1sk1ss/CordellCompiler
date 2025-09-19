#include <ir/irgen.h>

int IR_generate(ir_gen_t* g, ir_ctx_t* ctx) {
    scope_reset(&ctx->heap);
    g->funcdef(ctx->synt->r, g, ctx);
    g->blockgen(ctx->synt->r, g, ctx);
    return 1;
}
