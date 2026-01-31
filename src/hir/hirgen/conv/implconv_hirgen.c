#include <hir/hirgens/hirgens.h>

hir_subject_t* HIR_generate_implconv(hir_ctx_t* ctx, char ptr, hir_subject_type_t t, hir_subject_t* src, sym_table_t* smt) {
    if (
        HIR_similar_type(t, src->t) && /* This types are similar   */
        (
            src->ptr == ptr ||         /* Reference level the same */
            (
                (src->ptr > 0 || HIR_get_type_size(src->t) == 8) && 
                (ptr > 0 || HIR_get_type_size(t) == 8)
            ) /* Or reference level says that there isn't necessary to convert */
        )
    ) return src;

    token_flags_t cnv_flags = { .ptr = ptr };
    hir_subject_t* cnv = HIR_SUBJ_TMPVAR(t, VRTB_add_info(NULL, HIR_get_tmptkn_type(t), 0, &cnv_flags, &smt->v));
    cnv->ptr = ptr;
     
    /* If this is a pointer convertion (something to a pointer),
       the convertion becomes a basic 'as u64' cast. */
    hir_operation_t op = HIR_convop(t);
    if (cnv->ptr > 0) op = HIR_TU64;

    HIR_BLOCK2(ctx, op, cnv, src);
    return cnv;
}
