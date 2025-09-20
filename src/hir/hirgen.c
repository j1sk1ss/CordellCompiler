#include <hir/hirgen.h>

int HIR_generate(hir_ctx_t* ctx) {
    return HIR_generate_block(ctx->synt->r, ctx);
}
