#include <generator.h>

int x86_64_generate_store(ast_node_t* node, FILE* output, gen_ctx_t* ctx) {
    if (!node->token) return 0;
    if (VRS_isptr(node->token)) {
        iprintf(output, "mov qword ptr %s, rax\n", GET_ASMVAR(node));
        if (node->child) goto indexing;
        return 1;
    }

    switch (node->token->t_type) {
        case LONG_VARIABLE_TOKEN: 
            iprintf(output, "mov qword ptr %s, rax\n", GET_ASMVAR(node));
        break;
        case INT_VARIABLE_TOKEN:  
            iprintf(output, "mov word ptr %s, eax\n", GET_ASMVAR(node));
        break;
        case SHORT_VARIABLE_TOKEN:
            iprintf(output, "mov word ptr %s, ax\n", GET_ASMVAR(node));
        break;
        case CHAR_VARIABLE_TOKEN: 
            iprintf(output, "mov byte ptr %s, al\n", GET_ASMVAR(node));
        break;
        case ARR_VARIABLE_TOKEN:
        case STR_VARIABLE_TOKEN: {
indexing:
            ast_node_t* off = node->child;
            if (off) { /* Loading data from array by offset */
                iprintf(output, "mov rdx, rax\n");

                array_info_t arr_info = { .el_size = 1 };
                ARM_get_info(node->token->value, node->info.s_id, &arr_info, ctx->synt->arrs);
                int elsize = MAX(VRS_variable_bitness(node->token, 0) / 8, arr_info.el_size);

                x86_64_generate_block(off, output, ctx);
                if (elsize > 1) {
                    iprintf(output, "imul rax, %d\n", elsize);
                }
                
                iprintf(output, "lea rbx, %s\n", GET_ASMVAR(node));
                iprintf(output, "add rax, rbx\n");

                switch (elsize) {
                    case 1: iprintf(output, "mov byte ptr [rax], rdx\n");  break;
                    case 2: iprintf(output, "mov word ptr [rax], dx\n");   break;
                    case 4: iprintf(output, "mov dword ptr [rax], rdx\n"); break;
                    case 8: iprintf(output, "mov qword ptr [rax], rdx\n"); break;
                }
            }
            else { /* Loading array address to rax */
                iprintf(output, "lea %s, rax\n", GET_ASMVAR(node));
            }

            break;
        }
        
    }

    return 1;
}