#include <hir/hirgens/hirgens.h>

int HIR_generate_breakpoint_block(ast_node_t* node, hir_ctx_t* ctx) {
    return HIR_BLOCK1(ctx, HIR_BREAKPOINT, node->c ? HIR_SUBJ_STRING(node->c) : NULL);
}
