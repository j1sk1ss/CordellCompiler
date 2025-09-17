#include <x86_64_gnu_nasm.h>

static int _deref_rbx(int elsize, FILE* output) {
    switch (elsize) {
        case 1: iprintf(output, "mov byte [rbx], al\n");  break;
        case 2: iprintf(output, "mov word [rbx], ax\n");   break;
        case 4: iprintf(output, "mov dword [rbx], eax\n"); break;
        case 8: iprintf(output, "mov qword [rbx], rax\n"); break;
    }

    return 1;
}

int x86_64_generate_store(ast_node_t* node, FILE* output, gen_ctx_t* ctx, gen_t* g) {
    if (!node->token) return 0;
    if (VRS_isptr(node->token)) {
        if (node->child) goto indexing;
        else {
            if (!node->token->flags.dref) iprintf(output, "mov qword %s, rax\n", GET_ASMVAR(node));
            else {
                array_info_t arr_info = { .el_size = 1 };
                ART_get_info(node->token->value, node->sinfo.s_id, &arr_info, ctx->synt->symtb.arrs);
                int elsize = MAX(VRS_variable_bitness(node->token, 0) / 8, arr_info.el_size);
                iprintf(output, "mov rbx, %s\n", GET_ASMVAR(node));
                _deref_rbx(elsize, output);
            }
        }

        return 1;
    }

    switch (node->token->t_type) {
        case F64_VARIABLE_TOKEN:
        case I64_VARIABLE_TOKEN: 
        case U64_VARIABLE_TOKEN: 
            iprintf(output, "mov qword %s, rax\n", GET_ASMVAR(node));
        break;
        case F32_VARIABLE_TOKEN:
        case I32_VARIABLE_TOKEN:
        case U32_VARIABLE_TOKEN:  
            iprintf(output, "mov dword %s, eax\n", GET_ASMVAR(node));
        break;
        case I16_VARIABLE_TOKEN:
        case U16_VARIABLE_TOKEN:
            iprintf(output, "mov word %s, ax\n", GET_ASMVAR(node));
        break;
        case I8_VARIABLE_TOKEN:
        case U8_VARIABLE_TOKEN: 
            iprintf(output, "mov byte %s, al\n", GET_ASMVAR(node));
        break;
        case ARR_VARIABLE_TOKEN:
        case STR_VARIABLE_TOKEN: {
indexing: {}
            ast_node_t* off = node->child;
            if (off) {
                iprintf(output, "mov rdx, rax\n");

                array_info_t arr_info = { .el_size = 1 };
                ART_get_info(node->token->value, node->sinfo.s_id, &arr_info, ctx->synt->symtb.arrs);
                int elsize = MAX(VRS_variable_bitness(node->token, 0) / 8, arr_info.el_size);

                g->elemegen(off, output, ctx, g);
                if (elsize > 1) {
                    iprintf(output, "imul rax, %d\n", elsize);
                }

                if (!node->token->flags.ptr && !node->token->flags.heap) iprintf(output, "lea rbx, %s\n", GET_ASMVAR(node));
                else iprintf(output, "mov rbx, %s\n", GET_ASMVAR(node));
                iprintf(output, "add rax, rbx\n");
                _deref_rbx(elsize, output);
            }

            break;
        }
        default: break;
    }

    return 1;
}