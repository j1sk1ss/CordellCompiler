#include <ir/gens/irgen.h>

int x86_64_generate_operand(ast_node_t* node, ir_get_t* g, ir_ctx_t* ctx) {
    ast_node_t* op    = node;
    ast_node_t* left  = node->child;
    ast_node_t* right = left->sibling;

    int simd = VRS_is_float(left->token) || VRS_is_float(right->token);
    if (VRS_instant_movable(left->token)) IR_BLOCK2(ctx, iMOV, IR_SUBJ_REG(RBX), IR_SUBJ_VAR(left));
    else {
        g->elemegen(left, g, ctx);
        IR_BLOCK2(ctx, iMOV, IR_SUBJ_REG(RBX), IR_SUBJ_REG(RAX));
    }

    if (simd) {
        if (VRS_is_float(left->token)) IR_BLOCK2(ctx, iMOVq, IR_SUBJ_REG(XMM1), IR_SUBJ_REG(RBX));
        else IR_BLOCK2(ctx, TDBL, IR_SUBJ_REG(XMM1), IR_SUBJ_REG(RBX));
    }

    if (VRS_instant_movable(right->token)) IR_BLOCK2(ctx, iMOV, IR_SUBJ_REG(RAX), IR_SUBJ_VAR(left));
    else {
        IR_BLOCK1(ctx, PUSH, IR_SUBJ_REG(RBX));
        g->elemegen(right, g, ctx);
        IR_BLOCK1(ctx, POP, IR_SUBJ_REG(RBX));
    }
    
    if (simd) {
        if (VRS_is_float(left->token)) IR_BLOCK2(ctx, iMOVq, IR_SUBJ_REG(XMM1), IR_SUBJ_REG(RAX));
        else IR_BLOCK2(ctx, TDBL, IR_SUBJ_REG(XMM1), IR_SUBJ_REG(RAX));
    }

    switch (op->token->t_type) {
        case BITMOVE_LEFT_TOKEN:
        case BITMOVE_RIGHT_TOKEN: {
            IR_BLOCK2(ctx, iMOV, IR_SUBJ_REG(RCX), IR_SUBJ_REG(RAX));
            if (VRS_issign(right->token) && VRS_issign(left->token)) {
                IR_BLOCK2(ctx, node->token->t_type == BITMOVE_LEFT_TOKEN ? bSHL : bSHR, IR_SUBJ_REG(RBX), IR_SUBJ_REG(CL));
            }
            else {
                IR_BLOCK2(ctx, node->token->t_type == BITMOVE_LEFT_TOKEN ? bSHL : bSAR, IR_SUBJ_REG(RBX), IR_SUBJ_REG(CL));
            }

            IR_BLOCK2(ctx, iMOV, IR_SUBJ_REG(RAX), IR_SUBJ_REG(RBX));
            break;
        }
        case BITOR_TOKEN:
        case BITAND_TOKEN:
        case BITXOR_TOKEN: {
            if (node->token->t_type == BITAND_TOKEN)     IR_BLOCK2(ctx, bAND, IR_SUBJ_REG(RAX), IR_SUBJ_REG(RBX));
            else if (node->token->t_type == BITOR_TOKEN) IR_BLOCK2(ctx, bOR, IR_SUBJ_REG(RAX), IR_SUBJ_REG(RBX));
            else                                         IR_BLOCK2(ctx, bXOR, IR_SUBJ_REG(RAX), IR_SUBJ_REG(RBX));
            break;
        }
        case OR_TOKEN:
        case AND_TOKEN: {
            int lbl_id = ctx->lid++;
            IR_BLOCK2(ctx, TST, IR_SUBJ_REG(RBX), IR_SUBJ_REG(RBX));
            if (node->token->t_type == AND_TOKEN)  IR_BLOCK1(ctx, JE, IR_SUBJ_STR("_f%d\n", lbl_id));
            else IR_BLOCK1(ctx, JNE, IR_SUBJ_STR("_t%d\n", lbl_id));

            g->elemegen(right, g, ctx);
            IR_BLOCK2(ctx, TST, IR_SUBJ_REG(RAX), IR_SUBJ_REG(RAX));
            IR_BLOCK1(ctx, JNE, IR_SUBJ_STR("_t%d\n", lbl_id));
            IR_BLOCK1(ctx, JMP, IR_SUBJ_STR("_f%d\n", lbl_id));

            IR_BLOCK1(ctx, MKLB, IR_SUBJ_STR("_t%d:\n", lbl_id));
            IR_BLOCK2(ctx, iMOV, IR_SUBJ_REG(RAX), IR_SUBJ_CNST(1));
            IR_BLOCK1(ctx, JMP, IR_SUBJ_STR("_e%d\n", lbl_id));
            
            IR_BLOCK1(ctx, MKLB, IR_SUBJ_STR("_f%d:\n", lbl_id));
            IR_BLOCK2(ctx, iMOV, IR_SUBJ_REG(RAX), IR_SUBJ_CNST(0));
            IR_BLOCK1(ctx, MKLB, IR_SUBJ_STR("_e%d:\n", lbl_id));
            break;
        }
        case ADDASSIGN_TOKEN:
        case PLUS_TOKEN: {
            if (!simd) IR_BLOCK2(ctx, iADD, IR_SUBJ_REG(RAX), IR_SUBJ_REG(RBX));
            else {
                IR_BLOCK2(ctx, fADD, IR_SUBJ_REG(XMM0), IR_SUBJ_REG(XMM1));
                IR_BLOCK2(ctx, iMOVq, IR_SUBJ_REG(RAX), IR_SUBJ_REG(XMM0));
            }
            
            break;
        }
        case SUBASSIGN_TOKEN:
        case MINUS_TOKEN: {
            if (!simd) {
                IR_BLOCK2(ctx, iSUB, IR_SUBJ_REG(RBX), IR_SUBJ_REG(RAX));
                IR_BLOCK2(ctx, iMOV, IR_SUBJ_REG(RAX), IR_SUBJ_REG(RBX));
            }
            else {
                IR_BLOCK2(ctx, fSUB, IR_SUBJ_REG(XMM1), IR_SUBJ_REG(XMM0));
                IR_BLOCK2(ctx, fMVf, IR_SUBJ_REG(XMM0), IR_SUBJ_REG(XMM1));
                IR_BLOCK2(ctx, iMOVq, IR_SUBJ_REG(RAX), IR_SUBJ_REG(XMM0));
            }

            break;
        }
        case MULASSIGN_TOKEN:
        case MULTIPLY_TOKEN: {
            if (!simd) IR_BLOCK2(ctx, iMUL, IR_SUBJ_REG(RAX), IR_SUBJ_REG(RBX));
            else {
                IR_BLOCK2(ctx, fMUL, IR_SUBJ_REG(XMM0), IR_SUBJ_REG(XMM1));
                IR_BLOCK2(ctx, iMOVq, IR_SUBJ_REG(RAX), IR_SUBJ_REG(XMM0));
            }

            break;
        }
        case DIVASSIGN_TOKEN:
        case DIVIDE_TOKEN: {
            IR_BLOCK2(ctx, XCHG, IR_SUBJ_REG(RAX), IR_SUBJ_REG(RBX));
            if (VRS_issign(right->token) && VRS_issign(left->token)) {
                IR_BLOCK0(ctx, CDQ);
                IR_BLOCK1(ctx, iDIV, IR_SUBJ_REG(RBX));
            } 
            else {
                if (!simd) {
                    IR_BLOCK2(ctx, bXOR, IR_SUBJ_REG(RDX), IR_SUBJ_REG(RDX));
                    IR_BLOCK1(ctx, DIV, IR_SUBJ_REG(RBX));
                }
                else {
                    IR_BLOCK2(ctx, fDIV, IR_SUBJ_REG(XMM1), IR_SUBJ_REG(XMM0));
                    IR_BLOCK2(ctx, fMVf, IR_SUBJ_REG(XMM0), IR_SUBJ_REG(XMM1));
                    IR_BLOCK2(ctx, iMOVq, IR_SUBJ_REG(RAX), IR_SUBJ_REG(XMM0));
                }
            }

            break;
        }
        case MODULO_TOKEN: {
            IR_BLOCK2(ctx, XCHG, IR_SUBJ_REG(RAX), IR_SUBJ_REG(RBX));
            if (VRS_issign(right->token) && VRS_issign(left->token)) {
                IR_BLOCK0(ctx, CDQ);
                IR_BLOCK1(ctx, iDIV, IR_SUBJ_REG(RBX));
            } 
            else {
                IR_BLOCK2(ctx, bXOR, IR_SUBJ_REG(RDX), IR_SUBJ_REG(RDX));
                IR_BLOCK1(ctx, DIV, IR_SUBJ_REG(RBX));
            }

            IR_BLOCK2(ctx, iMOV, IR_SUBJ_REG(RAX), IR_SUBJ_REG(RDX));
            break;
        }
        case LOWER_TOKEN:
        case LARGER_TOKEN:
        case LOWEREQ_TOKEN:
        case COMPARE_TOKEN:
        case LARGEREQ_TOKEN:
        case NCOMPARE_TOKEN: {
            if (simd) iprintf(output, "ucomisd xmm0, xmm1\n");
            else iprintf(output, "cmp rbx, rax\n");
            if (VRS_issign(right->token) && VRS_issign(left->token)) {
                if (node->token->t_type == LOWER_TOKEN)    iprintf(output, "setl al\n");
                if (node->token->t_type == LARGER_TOKEN)   iprintf(output, "setg al\n");
                if (node->token->t_type == LOWEREQ_TOKEN)  iprintf(output, "setle al\n");
                if (node->token->t_type == LARGEREQ_TOKEN) iprintf(output, "setge al\n");
                if (node->token->t_type == COMPARE_TOKEN)  iprintf(output, "sete al\n");
                if (node->token->t_type == NCOMPARE_TOKEN) iprintf(output, "setne al\n");
            } 
            else {
                if (node->token->t_type == LOWER_TOKEN)    iprintf(output, "setb al\n");
                if (node->token->t_type == LARGER_TOKEN)   iprintf(output, "seta al\n");
                if (node->token->t_type == LOWEREQ_TOKEN)  iprintf(output, "setbe al\n");
                if (node->token->t_type == LARGEREQ_TOKEN) iprintf(output, "setae al\n");
                if (node->token->t_type == COMPARE_TOKEN)  iprintf(output, "sete al\n");
                if (node->token->t_type == NCOMPARE_TOKEN) iprintf(output, "setne al\n");
            }

            iprintf(output, "movzx rax, al\n");
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
