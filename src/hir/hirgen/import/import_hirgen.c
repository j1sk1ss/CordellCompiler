#include <hir/hirgens/hirgens.h>

int HIR_generate_import_block(ast_node_t* node, hir_ctx_t* ctx) {
    for (ast_node_t* func = node->c->c; func; func = func->siblings.n) {
        HIR_BLOCK1(ctx, HIR_IMPORT, HIR_SUBJ_FUNCNAME(func));
    }

    return 1;
}