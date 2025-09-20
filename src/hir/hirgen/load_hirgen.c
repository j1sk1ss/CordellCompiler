#include <hir/hirgen/hirgen.h>

hir_subject_t* HIR_generate_load(ast_node_t* node, hir_ctx_t* ctx) {
    if (!node->token) return 0;

    hir_subject_t* res = NULL;
    if (node->token->flags.ptr) {
        if (node->child) goto indexing;
        else {
            if (!node->token->flags.dref) res = HIR_SUBJ_VAR(node->sinfo.offset, HIR_get_stktype(node->token));
            else {
                res = HIR_SUBJ_TMPVAR(HIR_get_tmptype_tkn(node->token));
                HIR_BLOCK2(ctx, DREF, res, HIR_SUBJ_VAR(node->sinfo.offset, HIR_get_stktype(node->token)));
            }
        }
    }
    else {
        switch (node->token->t_type) {
            case UNKNOWN_NUMERIC_TOKEN: res = HIR_SUBJ_NUMBER(node->token->value); break;
            case ARR_VARIABLE_TOKEN:
            case STR_VARIABLE_TOKEN: {
    indexing: {}
                ast_node_t* off = node->child;
                if (!off) res = HIR_SUBJ_VAR(node->sinfo.offset, HIR_get_stktype(node->token)); 
                else {
                    token_t tmp = { .t_type = node->token->t_type };
                    hir_subject_t* offt1 = HIR_generate_elem(off, ctx);
                    hir_subject_t* base  = HIR_SUBJ_VAR(node->sinfo.offset, HIR_get_stktype(node->token));
                    
                    array_info_t ai;
                    if (ART_get_info(node->token->value, node->sinfo.s_id, &ai, ctx->synt->symtb.arrs)) {
                        tmp.t_type = ai.el_type;
                    }

                    res = HIR_SUBJ_TMPVAR(HIR_get_tmptype_tkn(&tmp));
                    HIR_BLOCK3(ctx, INDEX, res, base, offt1);
                }

                break;
            }
            default: res = HIR_SUBJ_VAR(node->sinfo.offset, HIR_get_stktype(node->token)); break;
        }
    }

    if (node->token->flags.neg) {
        hir_subject_t* neg = HIR_SUBJ_TMPVAR(res->t);
        HIR_BLOCK2(ctx, NOT, neg, res);
        res = neg;
    }

    if (!res) res = HIR_SUBJ_CONST(0);
    return res;
}
