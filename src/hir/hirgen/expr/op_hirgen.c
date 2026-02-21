#include <hir/hirgens/hirgens.h>

hir_subject_t* HIR_generate_update_block(ast_node_t* node, hir_ctx_t* ctx, sym_table_t* smt, int ret) {
    ast_node_t* left  = node->c;
    ast_node_t* right = left->siblings.n;

    hir_subject_t* dst = HIR_generate_elem(left, ctx, smt);
    hir_subject_t* upd = HIR_generate_elem(right, ctx, smt);

    hir_subject_t* res = HIR_SUBJ_TMPVAR(dst->t, VRTB_add_info(NULL, HIR_get_tmptkn_type(dst->t), 0, NULL, &smt->v));
    upd = HIR_generate_implconv(ctx, res->ptr, res->t, upd, smt);
    
    switch (node->t->t_type) {
        case ADDASSIGN_TOKEN:    HIR_BLOCK3(ctx, HIR_iADD, res, dst, upd); break;
        case SUBASSIGN_TOKEN:    HIR_BLOCK3(ctx, HIR_iSUB, res, dst, upd); break;
        case MULASSIGN_TOKEN:    HIR_BLOCK3(ctx, HIR_iMUL, res, dst, upd); break;
        case DIVASSIGN_TOKEN:    HIR_BLOCK3(ctx, HIR_iDIV, res, dst, upd); break;
        case BITORASSIGN_TOKEN:  HIR_BLOCK3(ctx, HIR_bOR, res, dst, upd);  break;
        case MODULOASSIGN_TOKEN: HIR_BLOCK3(ctx, HIR_iMOD, res, dst, upd); break;
        case BITANDASSIGN_TOKEN: HIR_BLOCK3(ctx, HIR_bAND, res, dst, upd); break;
        case BITXORASSIGN_TOKEN: HIR_BLOCK3(ctx, HIR_bXOR, res, dst, upd); break;
        default: break;
    }
    
    HIR_generate_store_block(left, res, ctx, smt);
    return ret ? HIR_copy_subject(dst) : NULL;
}

hir_subject_t* HIR_generate_operand(ast_node_t* node, hir_ctx_t* ctx, sym_table_t* smt) {
    ast_node_t* op     = node;
    ast_node_t* left   = node->c;
    ast_node_t* right  = left->siblings.n;
    hir_subject_t* res = NULL;

    hir_subject_t* lt1 = HIR_generate_elem(left, ctx, smt);
    switch (op->t->t_type) {
        case OR_TOKEN: {
            hir_subject_t* true_lb  = HIR_SUBJ_LABEL();
            hir_subject_t* false_lb = HIR_SUBJ_LABEL();
            hir_subject_t* end_lb   = HIR_SUBJ_LABEL();
            HIR_BLOCK3(ctx, HIR_IFOP2, lt1, true_lb, false_lb);
            
            HIR_BLOCK1(ctx, HIR_MKLB, false_lb);
            hir_subject_t* lt2 = HIR_generate_elem(right, ctx, smt);
            res = HIR_SUBJ_TMPVAR(
                HIR_promote_types(lt1->t, lt2->t), 
                VRTB_add_info(NULL, HIR_get_tmptkn_type(HIR_promote_types(lt1->t, lt2->t)), 0, NULL, &smt->v)
            );
            
            lt2 = HIR_generate_implconv(ctx, res->ptr, res->t, lt2, smt);
            HIR_BLOCK2(ctx, HIR_STORE, res, lt2);
            HIR_BLOCK1(ctx, HIR_JMP, end_lb);
            HIR_BLOCK1(ctx, HIR_MKLB, true_lb);
            HIR_BLOCK2(ctx, HIR_STORE, res, HIR_SUBJ_CONST(1));
            HIR_BLOCK1(ctx, HIR_JMP, end_lb);
            HIR_BLOCK1(ctx, HIR_MKLB, end_lb);
            break;
        }

        case AND_TOKEN: {
            hir_subject_t* true_lb  = HIR_SUBJ_LABEL();
            hir_subject_t* false_lb = HIR_SUBJ_LABEL();
            hir_subject_t* end_lb   = HIR_SUBJ_LABEL();
            HIR_BLOCK3(ctx, HIR_IFOP2, lt1, true_lb, false_lb);

            HIR_BLOCK1(ctx, HIR_MKLB, true_lb);
            hir_subject_t* lt2 = HIR_generate_elem(right, ctx, smt);
            res = HIR_SUBJ_TMPVAR(
                HIR_promote_types(lt1->t, lt2->t), 
                VRTB_add_info(NULL, HIR_get_tmptkn_type(HIR_promote_types(lt1->t, lt2->t)), 0, NULL, &smt->v)
            );
            
            lt2 = HIR_generate_implconv(ctx, res->ptr, res->t, lt2, smt);
            HIR_BLOCK2(ctx, HIR_STORE, res, lt2);
            HIR_BLOCK1(ctx, HIR_JMP, end_lb);
            HIR_BLOCK1(ctx, HIR_MKLB, false_lb);
            HIR_BLOCK2(ctx, HIR_STORE, res, HIR_SUBJ_CONST(0));
            HIR_BLOCK1(ctx, HIR_JMP, end_lb);
            HIR_BLOCK1(ctx, HIR_MKLB, end_lb);
            break;
        }

        default: {
            hir_subject_t* lt2 = HIR_generate_elem(right, ctx, smt);
            res = HIR_SUBJ_TMPVAR(
                HIR_promote_types(lt1->t, lt2->t), 
                VRTB_add_info(NULL, HIR_get_tmptkn_type(HIR_promote_types(lt1->t, lt2->t)), 0, NULL, &smt->v)
            );
            
            lt1 = HIR_generate_implconv(ctx, res->ptr, res->t, lt1, smt);
            lt2 = HIR_generate_implconv(ctx, res->ptr, res->t, lt2, smt);
            switch (op->t->t_type) {
                case PLUS_TOKEN:          HIR_BLOCK3(ctx, HIR_iADD, res, lt1, lt2);  break;
                case BITOR_TOKEN:         HIR_BLOCK3(ctx, HIR_bOR, res, lt1, lt2);   break;
                case MINUS_TOKEN:         HIR_BLOCK3(ctx, HIR_iSUB, res, lt1, lt2);  break;
                case LOWER_TOKEN:         HIR_BLOCK3(ctx, HIR_iLWR, res, lt1, lt2);  break;
                case BITAND_TOKEN:        HIR_BLOCK3(ctx, HIR_bAND, res, lt1, lt2);  break;
                case BITXOR_TOKEN:        HIR_BLOCK3(ctx, HIR_bXOR, res, lt1, lt2);  break;
                case DIVIDE_TOKEN:        HIR_BLOCK3(ctx, HIR_iDIV, res, lt1, lt2);  break;
                case MODULO_TOKEN:        HIR_BLOCK3(ctx, HIR_iMOD, res, lt1, lt2);  break;
                case LARGER_TOKEN:        HIR_BLOCK3(ctx, HIR_iLRG, res, lt1, lt2);  break;
                case LOWEREQ_TOKEN:       HIR_BLOCK3(ctx, HIR_iLRE, res, lt1, lt2);  break;
                case COMPARE_TOKEN:       HIR_BLOCK3(ctx, HIR_iCMP, res, lt1, lt2);  break;
                case LARGEREQ_TOKEN:      HIR_BLOCK3(ctx, HIR_iLGE, res, lt1, lt2);  break;
                case NCOMPARE_TOKEN:      HIR_BLOCK3(ctx, HIR_iNMP, res, lt1, lt2);  break;
                case MULTIPLY_TOKEN:      HIR_BLOCK3(ctx, HIR_iMUL, res, lt1, lt2);  break;
                case BITMOVE_LEFT_TOKEN:  HIR_BLOCK3(ctx, HIR_iBLFT, res, lt1, lt2); break;
                case BITMOVE_RIGHT_TOKEN: HIR_BLOCK3(ctx, HIR_iBRHT, res, lt1, lt2); break;
                default: break;
            }

            break;
        }
    }

    return res;
}
