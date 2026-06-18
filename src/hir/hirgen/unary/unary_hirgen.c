#include <hir/hirgens/hirgens.h>

hir_subject_t* HIR_generate_unary(ast_node_t* node, hir_ctx_t* ctx, hir_operation_t op, sym_table_t* smt) {
    HIR_SET_CURRENT_POS(ctx, node);
    hir_subject_t* src = HIR_generate_elem(node->c, ctx, smt);
    hir_subject_t* neg = HIR_SUBJ_TMPVAR(src->t, VRTB_add_info(NULL, HIR_get_tmptkn_type(src->t), NO_SYMBOL_ID, EMPTY_BASIC_FLAGS, &smt->v));
    HIR_BLOCK2(ctx, op, neg, src);
    return neg;
}
