#include <hir/hirgens/hirgens.h>

hir_subject_t* HIR_generate_load(ast_node_t* node, hir_ctx_t* ctx, sym_table_t* smt) {
    if (!node->token) return 0;

    hir_subject_t* res = NULL;
    if (node->token->flags.ptr) {
        if (node->child) goto indexing;
        else {
            if (!node->token->flags.dref) res = HIR_SUBJ_ASTVAR(node);
            else {
                res = HIR_SUBJ_TMPVAR(HIR_get_tmptype_tkn(node->token, 0), VRTB_add_info(NULL, TMP_TYPE_TOKEN, 0, NULL, &smt->v));
                HIR_BLOCK2(ctx, HIR_GDREF, res, HIR_SUBJ_ASTVAR(node));
            }
        }
    }
    else {
        switch (node->token->t_type) {
            case STRING_VALUE_TOKEN:    res = HIR_SUBJ_STRING(node);               break;
            case UNKNOWN_NUMERIC_TOKEN: res = HIR_SUBJ_NUMBER(node->token->value); break;
            case ARR_VARIABLE_TOKEN:
            case STR_VARIABLE_TOKEN: {
    indexing: {}
                ast_node_t* off = node->child;
                if (!off) res = HIR_SUBJ_ASTVAR(node); 
                else {
                    token_t tmp = { .t_type = node->token->t_type };
                    hir_subject_t* offt1 = HIR_generate_elem(off, ctx, smt);
                    hir_subject_t* base  = HIR_SUBJ_ASTVAR(node);
                    
                    array_info_t ai;
                    if (ARTB_get_info(node->sinfo.v_id, &ai, &smt->a)) {
                        tmp.t_type = ai.el_type;
                    }

                    res = HIR_SUBJ_TMPVAR(HIR_get_tmptype_tkn(&tmp, 0), VRTB_add_info(NULL, TMP_TYPE_TOKEN, 0, NULL, &smt->v));
                    HIR_BLOCK3(ctx, HIR_GINDEX, res, base, offt1);
                }

                break;
            }
            default: res = HIR_SUBJ_ASTVAR(node); break;
        }
    }

    if (node->token->flags.ref) {
        hir_subject_t* ref = HIR_SUBJ_TMPVAR(HIR_TMPVARU64, VRTB_add_info(NULL, TMP_TYPE_TOKEN, 0, NULL, &smt->v));
        HIR_BLOCK2(ctx, HIR_REF, ref, res);
        res = ref;
    }

    if (node->token->flags.neg) {
        hir_subject_t* neg = HIR_SUBJ_TMPVAR(res->t, VRTB_add_info(NULL, TMP_TYPE_TOKEN, 0, NULL, &smt->v));
        HIR_BLOCK2(ctx, HIR_NOT, neg, res);
        res = neg;
    }

    if (!res) res = HIR_SUBJ_CONST(0);
    return res;
}
