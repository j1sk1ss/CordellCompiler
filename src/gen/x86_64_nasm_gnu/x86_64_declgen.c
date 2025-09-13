#include <x86_64_gnu_nasm.h>

static int _strdeclaration(ast_node_t* node, FILE* output, gen_ctx_t* ctx) {
    ast_node_t* name_node = node->child;
    ast_node_t* val_node  = name_node->sibling;

    char* s = val_node->token->value;
    int base_off = name_node->info.offset;
    while (*s) {
        iprintf(output, "mov byte [rbp - %d], %i\n", base_off--, *s);
        s++;
    }

    iprintf(output, "mov byte [rbp - %d], 0\n", base_off--);
    return 1;
}

static int _arrdeclaration(ast_node_t* node, FILE* output, gen_ctx_t* ctx, gen_t* g) {
    ast_node_t* name_node    = node->child;
    ast_node_t* size_node    = name_node->sibling;
    ast_node_t* el_size_node = size_node->sibling;
    ast_node_t* elems_node   = el_size_node->sibling;

    array_info_t arr_info = { .el_size = 1 };
    if (ART_get_info(name_node->token->value, name_node->info.s_id, &arr_info, ctx->synt->symtb.arrs)) {
        regs_t reg;
        get_reg(&reg, arr_info.el_size, RAX, 0);

        int base_off = node->info.offset;
        for (ast_node_t* t = elems_node; t; t = t->sibling) {
            if (VRS_isnumeric(t->token)) iprintf(output, "mov%s[rbp - %d], %s\n", reg.operation, base_off, GET_ASMVAR(t));
            else {
                g->elemegen(t, output, ctx, g);
                iprintf(output, "mov%s[rbp - %d], %s\n", reg.operation, base_off, reg.name);
            }

            base_off -= arr_info.el_size;
        }
    }

    return 1;
}

static int _stack_declaration(ast_node_t* node, FILE* output, gen_ctx_t* ctx, gen_t* g) {
    if (node->token->t_type == ARRAY_TYPE_TOKEN)    return _arrdeclaration(node, output, ctx, g);
    else if (node->token->t_type == STR_TYPE_TOKEN) return _strdeclaration(node, output, ctx);
    return 1;
}

int x86_64_generate_declaration(ast_node_t* node, FILE* output, gen_ctx_t* ctx, gen_t* g) {
    ast_node_t* name_node = node->child;
    if (!VRS_instack(name_node->token)) return 0;
    if (!VRS_one_slot(name_node->token)) {
        return _stack_declaration(node, output, ctx, g);
    }

    int val = 0;
    int is_const = 0;
    char* derictive = " ";

    ast_node_t* val_node = name_node->sibling;
    if (!VRS_isnumeric(val_node->token)) g->elemegen(val_node, output, ctx, g);
    else {
        is_const = 1;
        switch (VRS_variable_bitness(name_node->token, 1)) {
            default:
            case 64: derictive = " qword "; break;
            case 32: derictive = " dword "; break;
            case 16: derictive = " word ";  break;
            case 8:  derictive = " byte ";  break;
        }

        if (val_node->token->t_type == UNKNOWN_NUMERIC_TOKEN) {
            val = str_atoi(val_node->token->value);
        }
    }

    if (is_const) iprintf(output, "mov%s%s, %d\n", derictive, GET_ASMVAR(name_node), val);
    else iprintf(output, "mov%s%s, rax\n", derictive, GET_ASMVAR(name_node));
    return 1;
}
