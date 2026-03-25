#include <hir/hirgens/hirgens.h>

int HIR_generate_exit_block(ast_node_t* node, hir_ctx_t* ctx, sym_table_t* smt) {
    HIR_BLOCK1(ctx, HIR_SETPOS, HIR_SUBJ_LOCATION(&node->t->finfo));
    return HIR_BLOCK1(ctx, HIR_EXITOP, HIR_generate_elem(node->c, ctx, smt));
}