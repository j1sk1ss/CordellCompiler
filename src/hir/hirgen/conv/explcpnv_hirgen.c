#include <hir/hirgens/hirgens.h>

hir_subject_t* HIR_generate_explconv(ast_node_t* node, hir_ctx_t* ctx, sym_table_t* smt) {
    HIR_SET_CURRENT_POS(ctx, node);
    hir_subject_t* cnv = HIR_SUBJ_TMPVAR(
        HIR_get_tmptype_tkn(node->c->t, 0), 
        VRTB_add_info(NULL, node->c->t->t_type, NO_SYMBOL_ID, NULL, &smt->v)
    );

    cnv->ptr = node->c->t->flags.ptr;
    hir_operation_t op = HIR_get_convop(cnv->t);
    if (cnv->ptr > 0) op = HIR_TPTR;

    hir_subject_t* src = HIR_generate_elem(node->c->siblings.n, ctx, smt);
    HIR_BLOCK2(ctx, op, cnv, src);
    return cnv;
}
