#include <generator.h>

/* Load to pointer destination */
int x86_64_generate_ptr_load(ast_node_t* node, FILE* output, gen_ctx_t* ctx) {
    if (!node->token) return 0;
    if (VRS_isptr(node->token)) goto stackable;
    switch (node->token->t_type) {
        case CHAR_VALUE_TOKEN:
            iprintf(output, "mov rax, %i\n", node->token->value[0]);
        break;
        case UNKNOWN_NUMERIC_TOKEN:
            iprintf(output, "mov rax, %s\n", node->token->value);
        break;
        case LONG_VARIABLE_TOKEN:
        case INT_VARIABLE_TOKEN:
        case SHORT_VARIABLE_TOKEN:
        case CHAR_VARIABLE_TOKEN:
            iprintf(output, "lea rax, %s\n", GET_ASMVAR(node));
        break;
        case ARR_VARIABLE_TOKEN:
        case STR_VARIABLE_TOKEN: {
stackable:
            ast_node_t* off = node->child;
            if (off) { /* Loading data from array by offset */
                array_info_t arr_info = { .el_size = 1 };
                ARM_get_info(node->token->value, node->info.s_id, &arr_info, ctx->synt->arrs);
                int elsize = MAX(VRS_variable_bitness(node->token, 0) / 8, arr_info.el_size);
                
                x86_64_generate_block(off, output, ctx);
                if (elsize > 1) {
                    iprintf(output, "imul rax, %d\n", elsize);
                }
                
                iprintf(output, "lea rbx, %s\n", GET_ASMVAR(node));
                iprintf(output, "add rax, rbx\n");
            }
            else { /* Loading array address to rax */
                iprintf(output, "lea rax, %s\n", GET_ASMVAR(node));
            }

            break;
        }
    }

    return 1;
}

/* Load to destination */
int x86_64_generate_load(ast_node_t* node, FILE* output, gen_ctx_t* ctx) {
    if (!node->token) return 0;
    if (node->token->vinfo.ptr) {
        iprintf(output, "mov rax, %s\n", GET_ASMVAR(node));
        return 1;
    }

    switch (node->token->t_type) {
        case CHAR_VALUE_TOKEN:
            iprintf(output, "mov al, %i\n", node->token->value[0]);
        break;
        case UNKNOWN_NUMERIC_TOKEN:
            iprintf(output, "mov rax, %s\n", node->token->value);
        break;
        case LONG_VARIABLE_TOKEN:
            iprintf(output, "mov rax, %s\n", GET_ASMVAR(node));
        break;
        case INT_VARIABLE_TOKEN:
            iprintf(output, "mov eax, %s\n", GET_ASMVAR(node));
        break;
        case SHORT_VARIABLE_TOKEN:
            iprintf(output, "mov ax, %s\n", GET_ASMVAR(node));
        break;
        case CHAR_VARIABLE_TOKEN:
            iprintf(output, "mov al, %s\n", GET_ASMVAR(node));
        break;
        case ARR_VARIABLE_TOKEN:
        case STR_VARIABLE_TOKEN: {
            ast_node_t* off = node->child;
            if (off) { /* Loading data from array by offset */
                array_info_t arr_info = { .el_size = 1 };
                ARM_get_info(node->token->value, node->info.s_id, &arr_info, ctx->synt->arrs);
                int elsize = MAX(VRS_variable_bitness(node->token, 0) / 8, arr_info.el_size);

                x86_64_generate_block(off, output, ctx);
                if (elsize > 1) {
                    iprintf(output, "imul rax, %d\n", elsize);
                }
                
                iprintf(output, "lea rbx, %s\n", GET_ASMVAR(node));
                iprintf(output, "add rax, rbx\n");
                iprintf(output, "mov rax, [rax]\n");
            }
            else { /* Loading array address to rax */
                iprintf(output, "lea rax, %s\n", GET_ASMVAR(node));
            }

            break;
        }
    }

    return 1;
}
