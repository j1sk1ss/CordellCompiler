#include <hir/hirgens/hirgens.h>

int HIR_generate_extern_block(ast_node_t* node, hir_ctx_t* ctx) {
    return HIR_BLOCK1(ctx, HIR_OEXT, HIR_SUBJ_STRING(node));
}