#include <hir/hirgens/hirgens.h>

int HIR_generate_store_block(ast_node_t* node, hir_subject_t* src, hir_ctx_t* ctx, sym_table_t* smt) {
    if (TKN_isptr(node->t)) goto _indexing;
    switch (node->t->t_type) {
        case DREF_TYPE_TOKEN: HIR_generate_dref(node, ctx, smt, src); break;
        case ARR_VARIABLE_TOKEN:
        case STR_VARIABLE_TOKEN: {
_indexing: {}
            ast_node_t* off = node->c;
            hir_subject_t* trg = HIR_SUBJ_ASTVAR(node);
            if (!off) HIR_BLOCK2(ctx, HIR_STORE, trg, HIR_generate_implconv(ctx, trg->ptr, trg->t, src, smt));
            else {
                /* First we need to generate the basic information:
                   - offval - Offset value in the one virtual variable.
                   - vase - The variable for the base addres for an array / a pointer. */
                hir_subject_t* offval = HIR_generate_elem(off, ctx, smt);
                hir_subject_t* base   = HIR_reference_subject(trg, smt);

                /* Figure out if this is an array, a pointer or a heap array.
                   The difference:
                   - If this is an array, we must use the 'ref a + (x * el)' expression.
                   - Otherwise, we must use the 'a + (x + el) expression'. */
                array_info_t ai;
                hir_subject_type_t trg_type = trg->t;
                if (!ARTB_get_info(node->sinfo.v_id, &ai, &smt->a) || ai.heap) {
                    HIR_BLOCK2(ctx, HIR_STORE, base, trg);
                    base->ptr = MAX(base->ptr - 1, 0);
                }
                else {
                    token_t tmp = { .t_type = ai.elements_info.el_type };
                    HIR_BLOCK2(ctx, HIR_REF, base, trg);
                    trg_type = HIR_get_tmptype_tkn(&tmp, 0);
                }

                /* The final offset for the base address is the result of the
                   expression 'offset * element_size' */
                hir_subject_t* addr = HIR_SUBJ_TMPVAR(offval->t, VRTB_add_info(NULL, HIR_get_tmptkn_type(offval->t), 0, NULL, &smt->v));
                HIR_BLOCK3(
                    ctx, HIR_iMUL, addr, offval, 
                    HIR_generate_implconv(ctx, offval->ptr, offval->t, HIR_SUBJ_CONST(HIR_get_type_size(trg_type)), smt)
                );

                /* No we move the address (base) by the offser (addr):
                   - head = base + addr */
                hir_subject_t* head = HIR_SUBJ_TMPVAR(base->t, VRTB_add_info(NULL, HIR_get_tmptkn_type(base->t), 0, NULL, &smt->v));
                head->ptr = base->ptr;
                
                HIR_BLOCK3(ctx, HIR_iADD, head, base, HIR_generate_implconv(ctx, base->ptr, base->t, addr, smt));
                
                /* Load the input data to the moved head
                   *head = data */
                HIR_BLOCK2(ctx, HIR_LDREF, head, HIR_generate_implconv(ctx, trg->ptr - 1, trg_type, src, smt));
            }

            break;
        }
        
        /* The default case for a basic variable store */
        default: {
            hir_subject_t* trg = HIR_SUBJ_ASTVAR(node);
            HIR_BLOCK2(ctx, HIR_STORE, trg, HIR_generate_implconv(ctx, trg->ptr, trg->t, src, smt)); 
            break;
        }
    }

    return 1;
}