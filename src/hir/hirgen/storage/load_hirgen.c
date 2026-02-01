#include <hir/hirgens/hirgens.h>

hir_subject_t* HIR_generate_load(ast_node_t* node, hir_ctx_t* ctx, sym_table_t* smt) {
    hir_subject_t* res = NULL;
    if (node->t->flags.ptr) goto _indexing;
    else {
        switch (node->t->t_type) {
            case STRING_VALUE_TOKEN:          res = HIR_SUBJ_STRING(node);           break;
            case UNKNOWN_NUMERIC_TOKEN:       res = HIR_SUBJ_NUMBER(node->t->body);  break;
            case UNKNOWN_FLOAT_NUMERIC_TOKEN: res = HIR_SUBJ_FNUMBER(node->t->body); break;
            case ARR_VARIABLE_TOKEN:
            case STR_VARIABLE_TOKEN: {
_indexing: {}
                ast_node_t* off = node->c;
                hir_subject_t* src = HIR_SUBJ_ASTVAR(node);
                if (!off) res = src;
                else {
                    hir_subject_t* offval = HIR_generate_elem(off, ctx, smt);
                    hir_subject_t* base   = HIR_SUBJ_TMPVAR(HIR_TMPVARU64, VRTB_add_info(NULL, TMP_U64_TYPE_TOKEN, 0, NULL, &smt->v));
                    
                    /* Figure out if this is an array, a heap or a pointer.
                       - If this is a pointer or a heap (a heap is a pointer tho), just move it to the 'base',
                       - If this is an array, move a link to the array to the 'base' */
                    array_info_t ai;
                    hir_subject_type_t trg_type = src->t;
                    if (!ARTB_get_info(node->sinfo.v_id, &ai, &smt->a) || ai.heap) HIR_BLOCK2(ctx, HIR_STORE, base, src);
                    else {
                        token_t tmp = { .t_type = ai.elements_info.el_type };
                        HIR_BLOCK2(ctx, HIR_REF, base, src);
                        trg_type = HIR_get_tmptype_tkn(&tmp, 0);
                    }
                    
                    /* The final offset for the base address is the result of the
                       expression 'offset * element_size' */
                    hir_subject_t* addr = HIR_SUBJ_TMPVAR(offval->t, VRTB_add_info(NULL, HIR_get_tmptkn_type(offval->t), 0, NULL, &smt->v));
                    HIR_BLOCK3(
                        ctx, HIR_iMUL, addr, offval, 
                        HIR_generate_implconv(ctx, offval->ptr, offval->t, HIR_SUBJ_CONST(HIR_get_type_size(trg_type)), smt)
                    );

                    /* Now move the 'base' and obtain the 'head':
                       head = base + addr */
                    hir_subject_t* head = HIR_SUBJ_TMPVAR(base->t, VRTB_add_info(NULL, HIR_get_tmptkn_type(base->t), 0, NULL, &smt->v));
                    HIR_BLOCK3(ctx, HIR_iADD, head, base, HIR_generate_implconv(ctx, base->ptr, base->t, addr, smt));
                    
                    /* Assuming that this is a dereference operation (*ptr / ptr[0]),
                       we need to lower the .ptr flag. */
                    res = HIR_SUBJ_TMPVAR(trg_type, VRTB_add_info(NULL, HIR_get_tmptkn_type(trg_type), 0, NULL, &smt->v));
                    res->ptr = MAX(src->ptr - 1, 0);
                    
                    HIR_BLOCK2(ctx, HIR_GDREF, res, head);
                }

                break;
            }

            default: res = HIR_SUBJ_ASTVAR(node); break;
        }
    }

    return res ? res : HIR_SUBJ_CONST(0);
}
