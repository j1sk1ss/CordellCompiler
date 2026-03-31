#include <hir/hirgens/hirgens.h>

hir_subject_t* HIR_generate_implconv(hir_ctx_t* ctx, char ptr, hir_subject_type_t t, hir_subject_t* src, sym_table_t* smt) {
    if (
        HIR_similar_type(t, src->t) && /* This types are similar   */
        (
            src->ptr == ptr ||         /* Reference level the same */
            (
                (src->ptr > 0 || HIR_get_type_size(src->t) == CONF_get_full_bytness()) && 
                (ptr > 0 || HIR_get_type_size(t) == CONF_get_full_bytness())
            ) /* Or reference level says that there isn't necessary to convert */
        )
    ) return src;

    /* If this is a pointer convertion (something to a pointer),
       the convertion becomes a basic 'as ptr' cast. 
       We perform this given the nature of pointers (pointers have the same size). */
    hir_operation_t op = HIR_get_convop(t);
    if (ptr > 0) op = HIR_TPTR;

    if (HIR_get_type_size(src->t) > HIR_get_type_size(t) && op != HIR_TPTR) {
        HIRGEN_ERROR(ctx, "Narrow implicit cast is forbidden!");
        return src;
    }

    token_flags_t cnv_flags = { .ptr = ptr };
    hir_subject_t* cnv = HIR_SUBJ_TMPVAR(t, VRTB_add_info(NULL, HIR_get_tmptkn_type(t), NO_SYMBOL_ID, &cnv_flags, &smt->v));
    cnv->ptr = ptr;
    HIR_BLOCK2(ctx, op, cnv, src);
    return cnv;
}
