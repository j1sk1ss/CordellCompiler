#include <hir/hirgens/hirgens.h>

hir_subject_t* HIR_generate_explconv(ast_node_t* node, hir_ctx_t* ctx, sym_table_t* smt) {
    hir_subject_t* cnv = HIR_SUBJ_TMPVAR(
        HIR_get_tmptype_tkn(node->c->t, 0), 
        VRTB_add_info(NULL, node->c->t->t_type, 0, NULL, &smt->v)
    );

    cnv->ptr = node->c->t->flags.ptr;
    hir_operation_t op = HIR_convop(cnv->t);
    if (cnv->ptr > 0) op = HIR_TPTR;

    hir_subject_t* src = HIR_generate_elem(node->c->siblings.n, ctx, smt);
    HIR_BLOCK2(ctx, op, cnv, src);
    return cnv;
}
