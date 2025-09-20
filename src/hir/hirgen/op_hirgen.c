#include <hir/hirgen/hirgen.h>

hir_subject_t* HIR_generate_operand_block(ast_node_t* node, hir_ctx_t* ctx) {
    ast_node_t* op = node;
    ast_node_t* left = node->child;
    ast_node_t* right = left->sibling;
    int simd = VRS_is_float(left->token) || VRS_is_float(right->token);

    hir_subject_t* lt1 = HIR_generate_elem_block(left, ctx);
    hir_subject_t* lt2 = HIR_generate_elem_block(right, ctx);
    hir_subject_t* res = HIR_SUBJ_TMPVAR(HIR_promote_types(lt1->t, lt2->t));

    switch (op->token->t_type) {
        case BITMOVE_LEFT_TOKEN:  HIR_BLOCK3(ctx, iBLFT, res, lt1, lt2); break;
        case BITMOVE_RIGHT_TOKEN: HIR_BLOCK3(ctx, iBRHT, res, lt1, lt2); break;
        case BITOR_TOKEN:         HIR_BLOCK3(ctx, bOR, res, lt1, lt2);   break;
        case BITAND_TOKEN:        HIR_BLOCK3(ctx, bAND, res, lt1, lt2);  break;
        case BITXOR_TOKEN:        HIR_BLOCK3(ctx, bXOR, res, lt1, lt2);  break;
        case OR_TOKEN:            HIR_BLOCK3(ctx, iOR, res, lt1, lt2);   break;
        case AND_TOKEN:           HIR_BLOCK3(ctx, iAND, res, lt1, lt2);  break;
        case ADDASSIGN_TOKEN:
            mm_free(res);
            HIR_BLOCK3(ctx, iADD, lt1, lt1, lt2);
            res = lt1;
        break;
        case PLUS_TOKEN: HIR_BLOCK3(ctx, iADD, res, lt1, lt2); break;
        case SUBASSIGN_TOKEN:
            mm_free(res);
            HIR_BLOCK3(ctx, iSUB, lt1, lt1, lt2);
            res = lt1;
        break;
        case MINUS_TOKEN: HIR_BLOCK3(ctx, iSUB, res, lt1, lt2); break;
        case MULASSIGN_TOKEN:
            mm_free(res);
            HIR_BLOCK3(ctx, iMUL, lt1, lt1, lt2);
            res = lt1;
        break;
        case MULTIPLY_TOKEN: HIR_BLOCK3(ctx, iMUL, res, lt1, lt2); break;
        case DIVASSIGN_TOKEN:
            mm_free(res);
            HIR_BLOCK3(ctx, iDIV, lt1, lt1, lt2);
            res = lt1;
        break;
        case LOWER_TOKEN:    HIR_BLOCK3(ctx, iLWR, res, lt1, lt2); break;
        case DIVIDE_TOKEN:   HIR_BLOCK3(ctx, iDIV, res, lt1, lt2); break;
        case MODULO_TOKEN:   HIR_BLOCK3(ctx, iMOD, res, lt1, lt2); break;
        case LARGER_TOKEN:   HIR_BLOCK3(ctx, iLRG, res, lt1, lt2); break;
        case LOWEREQ_TOKEN:  HIR_BLOCK3(ctx, iLRE, res, lt1, lt2); break;
        case COMPARE_TOKEN:  HIR_BLOCK3(ctx, iCMP, res, lt1, lt2); break;
        case LARGEREQ_TOKEN: HIR_BLOCK3(ctx, iLGE, res, lt1, lt2); break;
        case NCOMPARE_TOKEN: HIR_BLOCK3(ctx, iNMP, res, lt1, lt2); break;
        default: break;
    }
    
    return res;
}
