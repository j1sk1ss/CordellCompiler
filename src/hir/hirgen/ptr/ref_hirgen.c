#include <hir/hirgens/hirgens.h>

hir_subject_t* HIR_reference_subject(hir_subject_t* src, sym_table_t* smt, int increment) {
    /* We need to dereference the type of an element, 
       if this is an array type. */
    hir_subject_type_t src_type = src->t;
    int src_ptr = src->ptr;

    if (HIR_is_arrtype(src_type)) {
        array_info_t ai;
        if (ARTB_get_info(src->storage.var.v_id, &ai, &smt->a)) {
            token_t tmp = { .t_type = ai.elements_info.el_type };
            src_type = HIR_get_tmptype_tkn(&tmp, 0);
            src_ptr = ai.elements_info.el_flags.ptr;
        }
    }

    hir_subject_t* ref = HIR_SUBJ_TMPVAR(src_type, VRTB_add_info(NULL, HIR_get_tmptkn_type(src_type), 0, NULL, &smt->v));
    if (increment) ref->ptr = MAX(src_ptr + 1, 0);
    else ref->ptr = src_ptr;
    return ref;
}

hir_subject_t* HIR_generate_ref(ast_node_t* node, hir_ctx_t* ctx, sym_table_t* smt) {
    hir_subject_t* src = HIR_generate_elem(node->c, ctx, smt);
    hir_subject_t* ref = HIR_reference_subject(src, smt, 1);
    HIR_BLOCK2(ctx, HIR_REF, ref, src);
    return ref;
}
