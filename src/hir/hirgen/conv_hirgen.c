#include <hir/hirgens/hirgens.h>

hir_subject_t* HIR_generate_conv(hir_ctx_t* ctx, hir_subject_type_t t, hir_subject_t* src, sym_table_t* smt) {
    if (HIR_similar_type(t, src->t)) return src;
    hir_subject_t* cnv = HIR_SUBJ_TMPVAR(t, VRTB_add_info(NULL, HIR_get_tmptkn_type(t), 0, NULL, &smt->v));
    HIR_BLOCK2(ctx, HIR_convop(t), cnv, src);
    return cnv;
}
