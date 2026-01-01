#include <hir/hirgens/hirgens.h>

int HIR_generate_breakpoint_block(hir_ctx_t* ctx) {
    return HIR_BLOCK0(ctx, HIR_BREAKPOINT);
}
