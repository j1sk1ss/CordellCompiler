#include <generator.h>

int x86_64_generate_operand(ast_node_t* node, FILE* output, gen_ctx_t* ctx) {
    ast_node_t* op    = node;
    ast_node_t* left  = node->child;
    ast_node_t* right = left->sibling;

    x86_64_generate_block(left, output, ctx);

    switch (op->token->t_type) {
        case BITMOVE_LEFT_TOKEN:
        case BITMOVE_RIGHT_TOKEN: {
            iprintf(output, "push rax\n");
            x86_64_generate_block(right, output, ctx);
            iprintf(output, "pop rbx\n");
            iprintf(output, "mov rcx, rax\n");
            if (node->token->t_type == BITMOVE_LEFT_TOKEN) iprintf(output, "shl rax, cl\n");
            else iprintf(output, "shr rbx, cl\n");
            iprintf(output, "mov rax, rbx\n");
            break;
        }
        case BITAND_TOKEN:
        case BITOR_TOKEN:
        case BITXOR_TOKEN: {
            iprintf(output, "push rax\n");
            x86_64_generate_block(right, output, ctx);
            iprintf(output, "pop rbx\n");
            if (node->token->t_type == BITAND_TOKEN) iprintf(output, "and rax, rbx\n");
            else if (node->token->t_type == BITOR_TOKEN) iprintf(output, "or rax, rbx\n");
            else iprintf(output, "xor rax, rbx\n");
            break;
        }
        case AND_TOKEN:
        case OR_TOKEN: {
            int lbl_id = ctx->label++;
            iprintf(output, "cmp rax, 0\n");
            if (node->token->t_type == AND_TOKEN) iprintf(output, "je L_false_%d\n", lbl_id);
            else iprintf(output, "jne L_true_%d\n", lbl_id);

            x86_64_generate_block(right, output, ctx);
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
            iprintf(output, "push rax\n");
            x86_64_generate_block(right, output, ctx);
            iprintf(output, "pop rbx\n");
            iprintf(output, "add rax, rbx\n");
            break;
        }
        case MINUS_TOKEN: {
            iprintf(output, "push rax\n");
            x86_64_generate_block(right, output, ctx);
            iprintf(output, "pop rbx\n");
            iprintf(output, "sub rbx, rax\n");
            iprintf(output, "mov rax, rbx\n");
            break;
        }
        case MULTIPLY_TOKEN: {
            iprintf(output, "push rax\n");
            x86_64_generate_block(right, output, ctx);
            iprintf(output, "pop rbx\n");
            iprintf(output, "imul rax, rbx\n");
            break;
        }
        case DIVIDE_TOKEN: {
            iprintf(output, "push rax\n");
            x86_64_generate_block(right, output, ctx);
            iprintf(output, "mov rbx, rax\n");
            iprintf(output, "pop rax\n");
            iprintf(output, "cdq\n");
            iprintf(output, "idiv rbx\n");
            break;
        }
        case MODULO_TOKEN: {
            iprintf(output, "push rax\n");
            x86_64_generate_block(right, output, ctx);
            iprintf(output, "mov rbx, rax\n");
            iprintf(output, "pop rax\n");
            iprintf(output, "cdq\n");
            iprintf(output, "idiv rbx\n");
            iprintf(output, "mov rax, rdx\n");
            break;
        }
        case LARGER_TOKEN:
        case LOWER_TOKEN:
        case COMPARE_TOKEN:
        case NCOMPARE_TOKEN: {
            iprintf(output, "push rax\n");
            x86_64_generate_block(right, output, ctx);
            iprintf(output, "pop rbx\n");
            iprintf(output, "cmp rbx, rax\n");
            if (node->token->t_type == LARGER_TOKEN) iprintf(output, "setg al\n");
            else if (node->token->t_type == LOWER_TOKEN) iprintf(output, "setl al\n");
            else if (node->token->t_type == COMPARE_TOKEN) iprintf(output, "sete al\n");
            else iprintf(output, "setne al\n");
            iprintf(output, "movzx rax, al\n");
            break;
        }
    }

    return 1;
}
