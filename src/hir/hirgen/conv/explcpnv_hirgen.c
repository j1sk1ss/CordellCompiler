#include <hir/hirgens/hirgens.h>

hir_subject_t* HIR_generate_explconv(ast_node_t* node, hir_ctx_t* ctx, sym_table_t* smt) {
    hir_subject_t* cnv = HIR_SUBJ_TMPVAR(
        HIR_get_tmptype_tkn(node->c->t, node->c->t->flags.ptr), 
        VRTB_add_info(NULL, node->c->t->t_type, 0, NULL, &smt->v)
    );

    hir_subject_t* src = HIR_generate_elem(node->c->siblings.n, ctx, smt);
    HIR_BLOCK2(ctx, HIR_convop(cnv->t), cnv, src);
    return cnv;
}
