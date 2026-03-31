#include <hir/hirgens/hirgens.h>

int HIR_generate_break_block(ast_node_t* node, hir_ctx_t* ctx) {
    if (!ctx->carry.ptr) return 0;
    HIR_SET_CURRENT_POS(ctx, node);
    HIR_BLOCK0(ctx, HIR_BREAK);
    return HIR_BLOCK1(ctx, HIR_JMP, (hir_subject_t*)ctx->carry.ptr);
}
