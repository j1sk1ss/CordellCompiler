#include <hir/hirgens/hirgens.h>

int HIR_generate_break_block(hir_ctx_t* ctx) {
    if (!ctx->carry) return 0;
    return HIR_BLOCK1(ctx, HIR_JMP, (hir_subject_t*)ctx->carry);
}
