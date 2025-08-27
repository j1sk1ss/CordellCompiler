#include <generator.h>

static int _strdeclaration(ast_node_t* node, FILE* output, gen_ctx_t* ctx) {
    ast_node_t* name_node = node->child;
    ast_node_t* val_node  = name_node->sibling;

    char* string = val_node->token->value;
    int base_off = name_node->info.offset;
    while (*string) {
        iprintf(output, "mov byte [%s - %d], %i\n", GET_RAW_REG(BASE_BITNESS, RBP), base_off--, *string);
        string++;
    }

    return 1;
}

static int _arrdeclaration(ast_node_t* node, FILE* output, gen_ctx_t* ctx) {
    ast_node_t* name_node    = node->child;
    ast_node_t* size_node    = name_node->sibling;
    ast_node_t* el_size_node = size_node->sibling;
    ast_node_t* elems_node   = el_size_node->sibling;

    array_info_t arr_info = { .el_size = 1 };
    if (ARM_get_info(name_node->token->value, name_node->info.s_id, &arr_info, ctx->synt->arrs)) {
        regs_t reg;
        get_reg(&reg, arr_info.el_size, RAX, 0);

        int base_off = node->info.offset;
        for (ast_node_t* t = elems_node; t; t = t->sibling) {
            if (t->token->t_type == UNKNOWN_NUMERIC_TOKEN || t->token->t_type == CHAR_VALUE_TOKEN) {
                int val = t->token->t_type == UNKNOWN_NUMERIC_TOKEN ? str_atoi(t->token->value) : t->token->value[0];
                iprintf(
                    output, "mov%s[%s - %d], %d\n", reg.operation,
                    GET_RAW_REG(BASE_BITNESS, RBP), base_off, val
                );
            }
            else {
                x86_64_generate_elem(t, output, ctx);
                iprintf(output, "mov%s[%s - %d], %s\n", reg.operation, GET_RAW_REG(BASE_BITNESS, RBP), base_off, reg.name);
            }

            base_off -= arr_info.el_size;
        }
    }

    return 1;
}

static int _stack_declaration(ast_node_t* node, FILE* output, gen_ctx_t* ctx) {
    if (node->token->t_type == ARRAY_TYPE_TOKEN)    return _arrdeclaration(node, output, ctx);
    else if (node->token->t_type == STR_TYPE_TOKEN) return _strdeclaration(node, output, ctx);
    return 1;
}

int x86_64_generate_declaration(ast_node_t* node, FILE* output, gen_ctx_t* ctx) {
    ast_node_t* name_node = node->child;
    if (!VRS_intext(name_node->token)) return 0;
    if (!VRS_one_slot(name_node->token)) {
        return _stack_declaration(node, output, ctx);
    }

    int val = 0;
    int is_const = 0;
    char* derictive = " ";

    ast_node_t* val_node = name_node->sibling;
    if (
        val_node->token->t_type != UNKNOWN_NUMERIC_TOKEN && 
        val_node->token->t_type != CHAR_VALUE_TOKEN
    ) x86_64_generate_block(val_node, output, ctx);
    else {
        is_const = 1;
        switch (VRS_variable_bitness(name_node->token, 1)) {
            default:
            case 64: derictive = " qword "; break;
            case 32: derictive = " dword "; break;
            case 16: derictive = " word ";  break;
            case 8:  derictive = " byte ";  break;
        }

        if (val_node->token->t_type == UNKNOWN_NUMERIC_TOKEN) val = str_atoi(val_node->token->value);
        else if (val_node->token->t_type == CHAR_VALUE_TOKEN) val = val_node->token->value[0];
    }

    if (is_const) iprintf(output, "mov%s%s, %d\n", derictive, GET_ASMVAR(name_node), val);
    else iprintf(output, "mov%s%s, %s\n", derictive, GET_ASMVAR(name_node), GET_RAW_REG(BASE_BITNESS, RAX));
    return 1;
}
