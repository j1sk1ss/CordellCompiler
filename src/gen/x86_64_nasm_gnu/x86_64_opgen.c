#include <gen/x86_64_nasm_gnu/x86_64_gnu_nasm_asm.h>

int x86_64_generate_operand(ast_node_t* node, FILE* output, gen_ctx_t* ctx, gen_t* g) {
    ast_node_t* op    = node;
    ast_node_t* left  = node->child;
    ast_node_t* right = left->sibling;

    int simd = VRS_is_float(left->token) || VRS_is_float(right->token);
    if (VRS_instant_movable(left->token)) iprintf(output, "mov rbx, %s\n", GET_ASMVAR(left));
    else {
        g->elemegen(left, output, ctx, g);
        iprintf(output, "mov rbx, rax\n");
    }

    if (simd) {
        if (VRS_is_float(left->token)) iprintf(output, "movq xmm1, rbx\n");
        else iprintf(output, "cvtsi2sd xmm1, rbx\n");
    }

    if (VRS_instant_movable(right->token)) iprintf(output, "mov rax, %s\n", GET_ASMVAR(right));
    else {
        iprintf(output, "push rbx\n");
        g->elemegen(right, output, ctx, g);
        iprintf(output, "pop rbx\n");
    }
    
    if (simd) {
        if (VRS_is_float(left->token)) iprintf(output, "movq xmm0, rax\n");
        else iprintf(output, "cvtsi2sd xmm0, rax\n");
    }

    switch (op->token->t_type) {
        case BITMOVE_LEFT_TOKEN:
        case BITMOVE_RIGHT_TOKEN: {
            iprintf(output, "mov rcx, rax\n");
            if (VRS_issign(right->token) && VRS_issign(left->token)) {
                iprintf(output, "%s rbx, cl\n", node->token->t_type == BITMOVE_LEFT_TOKEN ? "shl" : "shr");
            }
            else {
                iprintf(output, "%s rbx, cl\n", node->token->t_type == BITMOVE_LEFT_TOKEN ? "shl" : "sar");
            }

            iprintf(output, "mov rax, rbx\n");
            break;
        }
        case BITOR_TOKEN:
        case BITAND_TOKEN:
        case BITXOR_TOKEN: {
            if (node->token->t_type == BITAND_TOKEN)     iprintf(output, "and rax, rbx\n");
            else if (node->token->t_type == BITOR_TOKEN) iprintf(output, "or rax, rbx\n");
            else                                         iprintf(output, "xor rax, rbx\n");
            break;
        }
        case OR_TOKEN:
        case AND_TOKEN: {
            int lbl_id = ctx->label++;
            iprintf(output, "test rbx, rbx\n");
            if (node->token->t_type == AND_TOKEN) iprintf(output, "je L_false_%d\n", lbl_id);
            else iprintf(output, "jne L_true_%d\n", lbl_id);

            g->elemegen(right, output, ctx, g);
            iprintf(output, "test rax, rax\n");
            iprintf(output, "jne L_true_%d\n", lbl_id);
            iprintf(output, "jmp L_false_%d\n", lbl_id);

            iprintf(output, "L_true_%d:\n", lbl_id);
            iprintf(output, "mov rax, 1\n");
            iprintf(output, "jmp L_end_%d\n", lbl_id);
            
            iprintf(output, "L_false_%d:\n", lbl_id);
            iprintf(output, "mov rax, 0\n");
            iprintf(output, "L_end_%d:\n", lbl_id);
            break;
        }
        case ADDASSIGN_TOKEN:
        case PLUS_TOKEN: {
            if (!simd) iprintf(output, "add rax, rbx\n"); 
            else {
                iprintf(output, "addsd xmm0, xmm1\n");
                iprintf(output, "movq rax, xmm0\n");
            }
            
            break;
        }
        case SUBASSIGN_TOKEN:
        case MINUS_TOKEN: {
            if (!simd) {
                iprintf(output, "sub rbx, rax\n");
                iprintf(output, "mov rax, rbx\n");
            }
            else {
                iprintf(output, "subsd xmm1, xmm0\n");
                iprintf(output, "movaps xmm0, xmm1\n");
                iprintf(output, "movq rax, xmm0\n");
            }

            break;
        }
        case MULASSIGN_TOKEN:
        case MULTIPLY_TOKEN: {
            if (!simd) iprintf(output, "imul rax, rbx\n");
            else {
                iprintf(output, "mulsd xmm0, xmm1\n");
                iprintf(output, "movq rax, xmm0\n");
            }

            break;
        }
        case DIVASSIGN_TOKEN:
        case DIVIDE_TOKEN: {
            iprintf(output, "xchg rax, rbx\n");
            if (VRS_issign(right->token) && VRS_issign(left->token)) {
                iprintf(output, "cdq\n");
                iprintf(output, "idiv rbx\n");
            } 
            else {
                if (!simd) {
                    iprintf(output, "xor rdx, rdx\n");
                    iprintf(output, "div rbx\n");
                }
                else {
                    iprintf(output, "divsd xmm1, xmm0\n");
                    iprintf(output, "movaps xmm0, xmm1\n");
                    iprintf(output, "movq rax, xmm0\n");
                }
            }

            break;
        }
        case MODULO_TOKEN: {
            iprintf(output, "xchg rax, rbx\n");
            if (VRS_issign(right->token) && VRS_issign(left->token)) {
                iprintf(output, "cdq\n");
                iprintf(output, "idiv rbx\n");
            } 
            else {
                iprintf(output, "xor rdx, rdx\n");
                iprintf(output, "div rbx\n");
            }

            iprintf(output, "mov rax, rdx\n");
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
    ) g->store(left, output, ctx, g);
    
    return 1;
}
