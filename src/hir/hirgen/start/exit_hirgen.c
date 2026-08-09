#include <hir/hirgens/hirgens.h>

int HIR_generate_exit_block(ast_node_t* node, hir_ctx_t* ctx, sym_table_t* smt) {
    HIR_SET_CURRENT_POS(ctx, node);
    hir_subject_t* rval = HIR_generate_elem(node->c, ctx, smt);
    if (!rval) rval = HIR_SUBJ_CONST(0);
    return HIR_BLOCK1(ctx, HIR_EXITOP, HIR_generate_implconv(ctx, 0, HIR_TMPVARU8, rval, smt));
}