#include <hir/hirgens/hirgens.h>

hir_subject_t* HIR_generate_poparg(ast_node_t* node, hir_ctx_t* ctx, sym_table_t* smt) {
    hir_subject_t* res = HIR_SUBJ_TMPVAR(HIR_TMPVARU64, VRTB_add_info(NULL, TKN_get_tmp_type(U64_TYPE_TOKEN), 0, NULL, &smt->v));
    HIR_BLOCK2(ctx, HIR_FARGLD, res, HIR_SUBJ_CONST(ctx->carry.val1++));
    return res;
}
