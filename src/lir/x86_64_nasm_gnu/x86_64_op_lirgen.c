#include <lir/x86_64_gnu_nasm/x86_64_irgen.h>

int LIR_generate_operand_block(ast_node_t* node, lir_gen_t* g, lir_ctx_t* ctx) {
    ast_node_t* op    = node;
    ast_node_t* left  = node->child;
    ast_node_t* right = left->sibling;

    int simd = VRS_is_float(left->token) || VRS_is_float(right->token);
    g->elemegen(left, g, ctx);
    IR_BLOCK2(ctx, iMOV, IR_SUBJ_REG(RBX, 8), IR_SUBJ_REG(RAX, 8));

    if (simd) {
        if (VRS_is_float(left->token)) IR_BLOCK2(ctx, iMOVq, IR_SUBJ_REG(XMM1, 8), IR_SUBJ_REG(RBX, 8));
        else IR_BLOCK2(ctx, TDBL, IR_SUBJ_REG(XMM1, 8), IR_SUBJ_REG(RBX, 8));
    }

    IR_BLOCK1(ctx, PUSH, IR_SUBJ_REG(RBX, 8));
    g->elemegen(right, g, ctx);
    IR_BLOCK1(ctx, POP, IR_SUBJ_REG(RBX, 8));
    
    if (simd) {
        if (VRS_is_float(left->token)) IR_BLOCK2(ctx, iMOVq, IR_SUBJ_REG(XMM1, 8), IR_SUBJ_REG(RAX, 8));
        else IR_BLOCK2(ctx, TDBL, IR_SUBJ_REG(XMM1, 8), IR_SUBJ_REG(RAX, 8));
    }

    switch (op->token->t_type) {
        case BITMOVE_LEFT_TOKEN:
        case BITMOVE_RIGHT_TOKEN: {
            IR_BLOCK2(ctx, iMOV, IR_SUBJ_REG(RCX, 8), IR_SUBJ_REG(RAX, 8));
            if (VRS_issign(right->token) && VRS_issign(left->token)) {
                IR_BLOCK2(ctx, node->token->t_type == BITMOVE_LEFT_TOKEN ? bSHL : bSHR, IR_SUBJ_REG(RBX, 8), IR_SUBJ_REG(CL, 1));
            }
            else {
                IR_BLOCK2(ctx, node->token->t_type == BITMOVE_LEFT_TOKEN ? bSHL : bSAR, IR_SUBJ_REG(RBX, 8), IR_SUBJ_REG(CL, 1));
            }

            IR_BLOCK2(ctx, iMOV, IR_SUBJ_REG(RAX, 8), IR_SUBJ_REG(RBX, 8));
            break;
        }
        case BITOR_TOKEN:
        case BITAND_TOKEN:
        case BITXOR_TOKEN: {
            if (node->token->t_type == BITAND_TOKEN)     IR_BLOCK2(ctx, bAND, IR_SUBJ_REG(RAX, 8), IR_SUBJ_REG(RBX, 8));
            else if (node->token->t_type == BITOR_TOKEN) IR_BLOCK2(ctx, bOR, IR_SUBJ_REG(RAX, 8), IR_SUBJ_REG(RBX, 8));
            else                                         IR_BLOCK2(ctx, bXOR, IR_SUBJ_REG(RAX, 8), IR_SUBJ_REG(RBX, 8));
            break;
        }
        case OR_TOKEN:
        case AND_TOKEN: {
            int lbl_id = ctx->lid++;
            IR_BLOCK2(ctx, TST, IR_SUBJ_REG(RBX, 8), IR_SUBJ_REG(RBX, 8));
            if (node->token->t_type == AND_TOKEN)  IR_BLOCK1(ctx, JE, IR_SUBJ_STR(8, "_f%d", lbl_id));
            else IR_BLOCK1(ctx, JNE, IR_SUBJ_STR(8, "_t%d", lbl_id));

            g->elemegen(right, g, ctx);
            IR_BLOCK2(ctx, TST, IR_SUBJ_REG(RAX, 8), IR_SUBJ_REG(RAX, 8));
            IR_BLOCK1(ctx, JNE, IR_SUBJ_STR(8, "_t%d", lbl_id));
            IR_BLOCK1(ctx, JMP, IR_SUBJ_STR(8, "_f%d", lbl_id));

            IR_BLOCK1(ctx, MKLB, IR_SUBJ_STR(8, "_t%d", lbl_id));
            IR_BLOCK2(ctx, iMOV, IR_SUBJ_REG(RAX, 8), IR_SUBJ_CNST(1));
            IR_BLOCK1(ctx, JMP, IR_SUBJ_STR(8, "_e%d", lbl_id));
            
            IR_BLOCK1(ctx, MKLB, IR_SUBJ_STR(8, "_f%d", lbl_id));
            IR_BLOCK2(ctx, iMOV, IR_SUBJ_REG(RAX, 8), IR_SUBJ_CNST(0));
            IR_BLOCK1(ctx, MKLB, IR_SUBJ_STR(8, "_e%d", lbl_id));
            break;
        }
        case ADDASSIGN_TOKEN:
        case PLUS_TOKEN: {
            if (!simd) IR_BLOCK2(ctx, iADD, IR_SUBJ_REG(RAX, 8), IR_SUBJ_REG(RBX, 8));
            else {
                IR_BLOCK2(ctx, fADD, IR_SUBJ_REG(XMM0, 8), IR_SUBJ_REG(XMM1, 8));
                IR_BLOCK2(ctx, iMOVq, IR_SUBJ_REG(RAX, 8), IR_SUBJ_REG(XMM0, 8));
            }
            
            break;
        }
        case SUBASSIGN_TOKEN:
        case MINUS_TOKEN: {
            if (!simd) {
                IR_BLOCK2(ctx, iSUB, IR_SUBJ_REG(RBX, 8), IR_SUBJ_REG(RAX, 8));
                IR_BLOCK2(ctx, iMOV, IR_SUBJ_REG(RAX, 8), IR_SUBJ_REG(RBX, 8));
            }
            else {
                IR_BLOCK2(ctx, fSUB, IR_SUBJ_REG(XMM1, 8), IR_SUBJ_REG(XMM0, 8));
                IR_BLOCK2(ctx, fMVf, IR_SUBJ_REG(XMM0, 8), IR_SUBJ_REG(XMM1, 8));
                IR_BLOCK2(ctx, iMOVq, IR_SUBJ_REG(RAX, 8), IR_SUBJ_REG(XMM0, 8));
            }

            break;
        }
        case MULASSIGN_TOKEN:
        case MULTIPLY_TOKEN: {
            if (!simd) IR_BLOCK2(ctx, iMUL, IR_SUBJ_REG(RAX, 8), IR_SUBJ_REG(RBX, 8));
            else {
                IR_BLOCK2(ctx, fMUL, IR_SUBJ_REG(XMM0, 8), IR_SUBJ_REG(XMM1, 8));
                IR_BLOCK2(ctx, iMOVq, IR_SUBJ_REG(RAX, 8), IR_SUBJ_REG(XMM0, 8));
            }

            break;
        }
        case DIVASSIGN_TOKEN:
        case DIVIDE_TOKEN: {
            IR_BLOCK2(ctx, XCHG, IR_SUBJ_REG(RAX, 8), IR_SUBJ_REG(RBX, 8));
            if (VRS_issign(right->token) && VRS_issign(left->token)) {
                IR_BLOCK0(ctx, CDQ);
                IR_BLOCK1(ctx, iDIV, IR_SUBJ_REG(RBX, 8));
            } 
            else {
                if (!simd) {
                    IR_BLOCK2(ctx, bXOR, IR_SUBJ_REG(RDX, 8), IR_SUBJ_REG(RDX, 8));
                    IR_BLOCK1(ctx, DIV, IR_SUBJ_REG(RBX, 8));
                }
                else {
                    IR_BLOCK2(ctx, fDIV, IR_SUBJ_REG(XMM1, 8), IR_SUBJ_REG(XMM0, 8));
                    IR_BLOCK2(ctx, fMVf, IR_SUBJ_REG(XMM0, 8), IR_SUBJ_REG(XMM1, 8));
                    IR_BLOCK2(ctx, iMOVq, IR_SUBJ_REG(RAX, 8), IR_SUBJ_REG(XMM0, 8));
                }
            }

            break;
        }
        case MODULO_TOKEN: {
            IR_BLOCK2(ctx, XCHG, IR_SUBJ_REG(RAX, 8), IR_SUBJ_REG(RBX, 8));
            if (VRS_issign(right->token) && VRS_issign(left->token)) {
                IR_BLOCK0(ctx, CDQ);
                IR_BLOCK1(ctx, iDIV, IR_SUBJ_REG(RBX, 8));
            } 
            else {
                IR_BLOCK2(ctx, bXOR, IR_SUBJ_REG(RDX, 8), IR_SUBJ_REG(RDX, 8));
                IR_BLOCK1(ctx, DIV, IR_SUBJ_REG(RBX, 8));
            }

            IR_BLOCK2(ctx, iMOV, IR_SUBJ_REG(RAX, 8), IR_SUBJ_REG(RDX, 8));
            break;
        }
        case LOWER_TOKEN:
        case LARGER_TOKEN:
        case LOWEREQ_TOKEN:
        case COMPARE_TOKEN:
        case LARGEREQ_TOKEN:
        case NCOMPARE_TOKEN: {
            if (simd) IR_BLOCK2(ctx, fCMP, IR_SUBJ_REG(XMM0, 8), IR_SUBJ_REG(XMM1, 8));
            else IR_BLOCK2(ctx, iCMP, IR_SUBJ_REG(RBX, 8), IR_SUBJ_REG(RAX, 8));
            if (VRS_issign(right->token) && VRS_issign(left->token)) {
                if (node->token->t_type == LOWER_TOKEN)    IR_BLOCK1(ctx, SETL, IR_SUBJ_REG(AL, 1));
                if (node->token->t_type == LARGER_TOKEN)   IR_BLOCK1(ctx, SETG, IR_SUBJ_REG(AL, 1)); 
                if (node->token->t_type == LOWEREQ_TOKEN)  IR_BLOCK1(ctx, STLE, IR_SUBJ_REG(AL, 1));
                if (node->token->t_type == LARGEREQ_TOKEN) IR_BLOCK1(ctx, STGE, IR_SUBJ_REG(AL, 1));
                if (node->token->t_type == COMPARE_TOKEN)  IR_BLOCK1(ctx, SETE, IR_SUBJ_REG(AL, 1)); 
                if (node->token->t_type == NCOMPARE_TOKEN) IR_BLOCK1(ctx, STNE, IR_SUBJ_REG(AL, 1));
            } 
            else {
                if (node->token->t_type == LOWER_TOKEN)    IR_BLOCK1(ctx, SETB, IR_SUBJ_REG(AL, 1)); 
                if (node->token->t_type == LARGER_TOKEN)   IR_BLOCK1(ctx, SETA, IR_SUBJ_REG(AL, 1)); 
                if (node->token->t_type == LOWEREQ_TOKEN)  IR_BLOCK1(ctx, STBE, IR_SUBJ_REG(AL, 1)); 
                if (node->token->t_type == LARGEREQ_TOKEN) IR_BLOCK1(ctx, STAE, IR_SUBJ_REG(AL, 1)); 
                if (node->token->t_type == COMPARE_TOKEN)  IR_BLOCK1(ctx, SETE, IR_SUBJ_REG(AL, 1)); 
                if (node->token->t_type == NCOMPARE_TOKEN) IR_BLOCK1(ctx, STNE, IR_SUBJ_REG(AL, 1)); 
            }

            IR_BLOCK2(ctx, iMVZX, IR_SUBJ_REG(RAX, 8), IR_SUBJ_REG(AL, 1));
            break;
        }
        default: break;
    }

    if (
        node->token->t_type == ADDASSIGN_TOKEN ||
        node->token->t_type == SUBASSIGN_TOKEN ||
        node->token->t_type == MULASSIGN_TOKEN ||
        node->token->t_type == DIVASSIGN_TOKEN
    ) g->store(left, g, ctx);
    
    return 1;
}
