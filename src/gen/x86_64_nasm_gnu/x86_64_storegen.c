#include <x86_64_gnu_nasm.h>

int x86_64_generate_store(ast_node_t* node, FILE* output, gen_ctx_t* ctx, gen_t* g) {
    if (!node->token) return 0;
    if (VRS_isptr(node->token)) {
        if (node->child) goto indexing;
        else {
            if (node->token->vinfo.dref) iprintf(output, "mov qword [%s], rax\n", GET_ASMVAR(node));
            else iprintf(output, "mov qword %s, rax\n", GET_ASMVAR(node));
        }

        return 1;
    }

    switch (node->token->t_type) {
        case LONG_VARIABLE_TOKEN: 
            iprintf(output, "mov qword %s, rax\n", GET_ASMVAR(node));
        break;
        case INT_VARIABLE_TOKEN:  
            iprintf(output, "mov dword %s, eax\n", GET_ASMVAR(node));
        break;
        case SHORT_VARIABLE_TOKEN:
            iprintf(output, "mov word %s, ax\n", GET_ASMVAR(node));
        break;
        case CHAR_VARIABLE_TOKEN: 
            iprintf(output, "mov byte %s, al\n", GET_ASMVAR(node));
        break;
        case ARR_VARIABLE_TOKEN:
        case STR_VARIABLE_TOKEN: {
indexing:
            ast_node_t* off = node->child;
            if (off) { /* Loading data from array by offset */
                iprintf(output, "mov rdx, rax\n");

                array_info_t arr_info = { .el_size = 1 };
                ART_get_info(node->token->value, node->info.s_id, &arr_info, ctx->synt->arrs);
                int elsize = MAX(VRS_variable_bitness(node->token, 0) / 8, arr_info.el_size);

                g->elemegen(off, output, ctx, g);
                if (elsize > 1) {
                    iprintf(output, "imul rax, %d\n", elsize);
                }

                if (!node->token->vinfo.ptr) iprintf(output, "lea rbx, %s\n", GET_ASMVAR(node));
                else iprintf(output, "mov rbx, %s\n", GET_ASMVAR(node));
                iprintf(output, "add rax, rbx\n");

                switch (elsize) {
                    case 1: iprintf(output, "mov [rax], dl\n");  break;
                    case 2: iprintf(output, "mov [rax], dx\n");   break;
                    case 4: iprintf(output, "mov [rax], edx\n"); break;
                    case 8: iprintf(output, "mov [rax], rdx\n"); break;
                }
            }
            else { /* Loading array address to rax */
                iprintf(output, "lea %s, rax\n", GET_ASMVAR(node));
            }

            break;
        }
        default: break;
    }

    return 1;
}