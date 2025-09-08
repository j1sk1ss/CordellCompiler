#include <x86_64_gnu_nasm.h>

int x86_64_generate_operand(ast_node_t* node, FILE* output, gen_ctx_t* ctx, gen_t* g) {
    ast_node_t* op    = node;
    ast_node_t* left  = node->child;
    ast_node_t* right = left->sibling;

    if (VRS_instant_movable(left->token)) iprintf(output, "mov rbx, %s\n", GET_ASMVAR(left));
    else {
        g->elemegen(left, output, ctx, g);
        iprintf(output, "mov rbx, rax\n");
    }

    if (VRS_instant_movable(right->token)) iprintf(output, "mov rax, %s\n", GET_ASMVAR(right));
    else g->elemegen(right, output, ctx, g);
    
    switch (op->token->t_type) {
        case BITMOVE_LEFT_TOKEN:
        case BITMOVE_RIGHT_TOKEN: {
            iprintf(output, "mov rcx, rax\n");
            if (VRS_issign(left->token)) {
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
            iprintf(output, "cmp rbx, 0\n");
            if (node->token->t_type == AND_TOKEN) iprintf(output, "je L_false_%d\n", lbl_id);
            else iprintf(output, "jne L_true_%d\n", lbl_id);

            g->elemegen(right, output, ctx, g);
            iprintf(output, "cmp rax, 0\n");
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
        case PLUS_TOKEN: {
            iprintf(output, "add rax, rbx\n");
            break;
        }
        case MINUS_TOKEN: {
            iprintf(output, "sub rbx, rax\n");
            iprintf(output, "mov rax, rbx\n");
            break;
        }
        case MULTIPLY_TOKEN: {
            iprintf(output, "imul rax, rbx\n");
            break;
        }
        case DIVIDE_TOKEN: {
            iprintf(output, "xchg rax, rbx\n");
            if (VRS_issign(left->token)) {
                iprintf(output, "cdq\n");
                iprintf(output, "idiv rbx\n");
            } 
            else {
                iprintf(output, "xor rdx, rdx\n");
                iprintf(output, "div rbx\n");
            }

            break;
        }
        case MODULO_TOKEN: {
            iprintf(output, "xchg rax, rbx\n");
            if (VRS_issign(left->token)) {
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
            iprintf(output, "cmp rbx, rax\n");
            if (VRS_issign(left->token)) {
                if (node->token->t_type == LOWER_TOKEN)    iprintf(output, "setl al\n");
                if (node->token->t_type == LARGER_TOKEN)   iprintf(output, "setg al\n");
                if (node->token->t_type == LOWEREQ_TOKEN)  iprintf(output, "setle al\n");
                if (node->token->t_type == LARGEREQ_TOKEN) iprintf(output, "setge al\n");
            } 
            else {
                if (node->token->t_type == LOWER_TOKEN)    iprintf(output, "setb al\n");
                if (node->token->t_type == LARGER_TOKEN)   iprintf(output, "seta al\n");
                if (node->token->t_type == LOWEREQ_TOKEN)  iprintf(output, "setbe al\n");
                if (node->token->t_type == LARGEREQ_TOKEN) iprintf(output, "setae al\n");
            }

            iprintf(output, "movzx rax, al\n");
            break;
        }
    }

    return 1;
}
