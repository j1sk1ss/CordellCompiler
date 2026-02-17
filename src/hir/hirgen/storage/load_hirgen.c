#include <hir/hirgens/hirgens.h>

hir_subject_t* HIR_generate_load(ast_node_t* node, hir_ctx_t* ctx, sym_table_t* smt) {
    hir_subject_t* res = NULL;
    switch (node->t->t_type) {
        case STRING_VALUE_TOKEN:          res = HIR_SUBJ_STRING(node);           break;
        case UNKNOWN_NUMERIC_TOKEN:       res = HIR_SUBJ_NUMBER(node->t->body);  break;
        case UNKNOWN_FLOAT_NUMERIC_TOKEN: res = HIR_SUBJ_FNUMBER(node->t->body); break;
        case CALL_ADDR: {
            res = HIR_SUBJ_TMPVAR(HIR_TMPVARU64, VRTB_add_info(NULL, TMP_U64_TYPE_TOKEN, 0, NULL, &smt->v));
            res->ptr = 1;
            HIR_BLOCK2(ctx, HIR_REF, res, HIR_SUBJ_FUNCNAME(node));
            break;
        }
        default: res = HIR_SUBJ_ASTVAR(node); break;
    }

    return res;
}
