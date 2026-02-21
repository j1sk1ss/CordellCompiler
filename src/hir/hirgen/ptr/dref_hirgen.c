#include <hir/hirgens/hirgens.h>

hir_subject_t* HIR_generate_dref(ast_node_t* node, hir_ctx_t* ctx, sym_table_t* smt, hir_subject_t* data) {
    hir_subject_t* src = HIR_generate_elem(node->c, ctx, smt);
    if (!data) {
        /* Generate the basic information 
           about a new one dereferenced value. */
        hir_subject_type_t dref_type = src->t;
        token_flags_t dref_flags = { 
            .ptr = MAX(src->ptr - 1, 0) 
        };

        /* If the base variable (the source) is an array,
           the dereferenced value type will be an array's element type. */
        array_info_t ai;
        if (ARTB_get_info(src->storage.var.v_id, &ai, &smt->a)) {
            token_t tmp = { .t_type = ai.elements_info.el_type };
            dref_type = HIR_get_tmptype_tkn(&tmp, 0);
            dref_flags.ptr = MAX(ai.elements_info.el_flags.ptr - 1, 0);
        }
        else {
            dref_type = HIR_get_tmp_type(src->t);
            dref_flags.ptr = MAX(src->ptr - 1, 0);
        }

        hir_subject_t* dref = HIR_SUBJ_TMPVAR(dref_type, VRTB_add_info(NULL, HIR_get_tmptkn_type(dref_type), 0, &dref_flags, &smt->v));
        dref->ptr = dref_flags.ptr;

        HIR_BLOCK2(ctx, HIR_GDREF, dref, src);
        return dref;
    }

    HIR_BLOCK2(ctx, HIR_LDREF, src, HIR_generate_implconv(ctx, src->ptr - 1, src->t, data, smt));
    return NULL;
}
