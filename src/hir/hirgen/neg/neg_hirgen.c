#include <hir/hirgens/hirgens.h>

hir_subject_t* HIR_generate_neg(ast_node_t* node, hir_ctx_t* ctx, sym_table_t* smt) {
    HIR_BLOCK1(ctx, HIR_SETPOS, HIR_SUBJ_LOCATION(&node->t->finfo));
    hir_subject_t* src = HIR_generate_elem(node->c, ctx, smt);
    hir_subject_t* neg = HIR_SUBJ_TMPVAR(src->t, VRTB_add_info(NULL, HIR_get_tmptkn_type(src->t), 0, NULL, &smt->v));
    HIR_BLOCK2(ctx, HIR_NOT, neg, src);
    return neg;
}
