#include <hir/hirgens/hirgens.h>

hir_subject_t* HIR_generate_load(ast_node_t* node, hir_ctx_t* ctx, sym_table_t* smt) {
    hir_subject_t* res = NULL;
    if (node->t->flags.ptr) {
        if (node->c) goto _indexing;
        else {
            if (!node->t->flags.dref) res = HIR_SUBJ_ASTVAR(node);
            else {
                res = HIR_SUBJ_TMPVAR(
                    HIR_get_tmptype_tkn(node->t, 0), 
                    VRTB_add_info(NULL, TKN_get_tmp_type(node->t->t_type), 0, NULL, &smt->v)
                );

                HIR_BLOCK2(ctx, HIR_GDREF, res, HIR_SUBJ_ASTVAR(node));
            }
        }
    }
    else {
        switch (node->t->t_type) {
            case STRING_VALUE_TOKEN:    res = HIR_SUBJ_STRING(node);              break;
            case UNKNOWN_NUMERIC_TOKEN: res = HIR_SUBJ_NUMBER(node->t->body); break;
            case ARR_VARIABLE_TOKEN:
            case STR_VARIABLE_TOKEN: {
_indexing: {}
                ast_node_t* off = node->c;
                if (!off) res = HIR_SUBJ_ASTVAR(node); 
                else {
                    hir_subject_t* offval = HIR_generate_elem(off, ctx, smt);
                    hir_subject_t* base   = HIR_SUBJ_TMPVAR(HIR_TMPVARU64, VRTB_add_info(NULL, TMP_U64_TYPE_TOKEN, 0, NULL, &smt->v));
                    
                    array_info_t ai;
                    token_t tmp = { .t_type = node->t->t_type };
                    if (
                        !ARTB_get_info(node->sinfo.v_id, &ai, &smt->a) ||
                        ai.heap
                    ) HIR_BLOCK2(ctx, HIR_STORE, base, HIR_SUBJ_ASTVAR(node));
                    else {
                        HIR_BLOCK2(ctx, HIR_REF, base, HIR_SUBJ_ASTVAR(node));
                        tmp.t_type = ai.el_type;
                    }
                    
                    hir_subject_t* addr = HIR_SUBJ_TMPVAR(offval->t, VRTB_add_info(NULL, HIR_get_tmptkn_type(offval->t), 0, NULL, &smt->v));
                    HIR_BLOCK3(
                        ctx, HIR_iMUL, addr, offval, 
                        HIR_generate_implconv(ctx, offval->t, HIR_SUBJ_CONST(HIR_get_type_size(HIR_get_tmptype_tkn(&tmp, 1))), smt)
                    );

                    hir_subject_t* head = HIR_SUBJ_TMPVAR(base->t, VRTB_add_info(NULL, HIR_get_tmptkn_type(base->t), 0, NULL, &smt->v));
                    HIR_BLOCK3(ctx, HIR_iADD, head, base, HIR_generate_implconv(ctx, base->t, addr, smt));
                    if (node->t->flags.ref) res = head;
                    else {
                        res = HIR_SUBJ_TMPVAR(HIR_get_tmptype_tkn(&tmp, 0), VRTB_add_info(NULL, TKN_get_tmp_type(tmp.t_type), 0, NULL, &smt->v));
                        HIR_BLOCK2(ctx, HIR_GDREF, res, head);
                    }
                    
                    goto _end;
                }

                break;
            }

            default: 
                res = HIR_SUBJ_ASTVAR(node); 
            break;
        }
    }

    if (node->t->flags.ref) {
        hir_subject_t* ref = HIR_SUBJ_TMPVAR(HIR_TMPVARU64, VRTB_add_info(NULL, TMP_U64_TYPE_TOKEN, 0, NULL, &smt->v));
        HIR_BLOCK2(ctx, HIR_REF, ref, res);
        res = ref;
    }

_end: {}
    if (node->t->flags.neg) {
        hir_subject_t* neg = HIR_SUBJ_TMPVAR(res->t, VRTB_add_info(NULL, HIR_get_tmptkn_type(res->t), 0, NULL, &smt->v));
        HIR_BLOCK2(ctx, HIR_NOT, neg, res);
        res = neg;
    }

    return res ? res : HIR_SUBJ_CONST(0);
}
