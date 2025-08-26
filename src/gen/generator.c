#include <generator.h>

static int _generate_expression(ast_node_t* node, FILE* output, const char* func, gen_ctx_t* ctx) {
    if (!node) return 0;
    if (node->token->t_type == IF_TOKEN)           _generate_if(node, output, func, ctx);
    else if (node->token->t_type == SWITCH_TOKEN)  _generate_switch(node, output, func, ctx);
    else if (node->token->t_type == WHILE_TOKEN)   _generate_while(node, output, func, ctx);
    else if (node->token->t_type == FUNC_TOKEN)    _generate_function(node, output, func, ctx);
    else if (node->token->t_type == SYSCALL_TOKEN) _generate_syscall(node, output, func, ctx);
    else if (node->token->t_type == ASSIGN_TOKEN)  _generate_assignment(node, output, func, ctx);
    else if (node->token->t_type == UNKNOWN_NUMERIC_TOKEN) iprintf(output, "mov %s, %s\n", GET_RAW_REG(BASE_BITNESS, RAX), node->token->value);
    else if (node->token->t_type == CHAR_VALUE_TOKEN)      iprintf(output, "mov %s, %i\n", GET_RAW_REG(8, RAX), *node->token->value);
    else if (node->token->vinfo.ptr && VRS_isdecl(node->token)) _generate_declaration(node, output, func, ctx);
    else if (node->token->vinfo.ptr && !VRS_isdecl(node->token) && VRS_isptr(node->token)) {
        if (!node->child) iprintf(output, "mov %s, %s\n", GET_RAW_REG(BASE_BITNESS, RAX), GET_ASMVAR(node));
        else {
            variable_info_t info;
            if (VRM_get_info((char*)node->token->value, func, &info, ctx->synt->vars)) {
                _generate_expression(node->child, output, func, ctx);

                int raw_bitness = VRS_variable_bitness(node->token, 0) / 8;
                if (raw_bitness > 1) iprintf(output, "imul %s, %d\n", GET_RAW_REG(BASE_BITNESS, RAX), raw_bitness);
                iprintf(output, "add %s, %s\n", GET_RAW_REG(BASE_BITNESS, RAX), GET_ASMVAR(node));

                regs_t reg;
                get_reg(&reg, raw_bitness, RAX, 0);
                iprintf(
                    output, "%s %s,%s[%s]\n", raw_bitness <= 2 ? "movzx" : "mov",
                    GET_RAW_REG(BASE_BITNESS, RAX), reg.operation, GET_RAW_REG(BASE_BITNESS, RAX)
                );
            }
        }
    }
    else if (node->token->t_type == LONG_TYPE_TOKEN)  _generate_declaration(node, output, func, ctx);
    else if (node->token->t_type == INT_TYPE_TOKEN)   _generate_declaration(node, output, func, ctx);
    else if (node->token->t_type == SHORT_TYPE_TOKEN) _generate_declaration(node, output, func, ctx);
    else if (node->token->t_type == CHAR_TYPE_TOKEN)  _generate_declaration(node, output, func, ctx);
    else if (
        node->token->t_type == CHAR_VARIABLE_TOKEN  ||
        node->token->t_type == SHORT_VARIABLE_TOKEN ||
        node->token->t_type == INT_VARIABLE_TOKEN   ||
        node->token->t_type == LONG_VARIABLE_TOKEN
    ) {
        iprintf(
            output, "mov %s, %s ; int %s\n", 
            GET_RAW_REG(VRS_variable_bitness(node->token, 1), RAX), GET_ASMVAR(node), node->token->value
        );

        _generate_expression(node->child, output, func, ctx);
    }
    else if (node->token->t_type == ARRAY_TYPE_TOKEN) {
        if (node->child && VRS_intext(node->token)) {
            ast_node_t* size   = node->child;
            ast_node_t* t_type = size->sibling;
            ast_node_t* name   = t_type->sibling;

            array_info_t arr_info = { .el_size = 1 };
            if (ARM_get_info((char*)name->token->value, func, &arr_info, ctx->synt->arrs)) {
                ast_node_t* vals = name->sibling;
                if (vals && vals->token->t_type != DELIMITER_TOKEN) {
                    fprintf(output, "\n ; --------------- Array setup %s --------------- \n", name->token->value);

                    regs_t reg;
                    get_reg(&reg, arr_info.el_size, RAX, 0);

                    int base_off = node->info.offset;
                    for (ast_node_t* v = vals; v && v->token->t_type != DELIMITER_TOKEN; v = v->sibling) {
                        if (v->token->t_type == UNKNOWN_NUMERIC_TOKEN) {
                            iprintf(
                                output, "mov%s[%s - %d], %d\n", 
                                reg.operation, GET_RAW_REG(BASE_BITNESS, RBP), base_off, str_atoi((char*)v->token->value)
                            );
                        }
                        else if (v->token->t_type == CHAR_VALUE_TOKEN) {
                            iprintf(
                                output, "mov%s[%s - %d], %d\n", 
                                reg.operation, GET_RAW_REG(BASE_BITNESS, RBP), base_off, *v->token->value
                            );
                        }
                        else {
                            int is_ptr = (
                                ARM_get_info((char*)v->token->value, func, NULL, ctx->synt->arrs) && 
                                VRS_intext(v->token)
                            );

                            iprintf(output, "%s %s, %s ; int %s \n", !is_ptr ? "mov" : "lea", reg.name, GET_ASMVAR(v), v->token->value);
                            iprintf(output, "mov%s[%s - %d], %s\n", reg.operation, GET_RAW_REG(BASE_BITNESS, RBP), base_off, reg.name);
                        }

                        base_off -= arr_info.el_size;
                    }

                    fprintf(output, " ; --------------- \n");
                }
            }
        }
    }
    else if (node->token->t_type == STR_TYPE_TOKEN) {
        ast_node_t* name_node = node->child;
        if (name_node->sibling && VRS_intext(name_node->token)) {
            fprintf(output, "\n ; --------------- String setup %s --------------- \n", name_node->token->value);
            ast_node_t* val_node = name_node->sibling;
            char* val_head = (char*)val_node->token->value;
            int base_off = name_node->info.offset;
            while (*val_head) {
                iprintf(output, "mov byte [%s - %d], %i\n", GET_RAW_REG(BASE_BITNESS, RBP), base_off--, *val_head);
                val_head++;
            }

            fprintf(output, " ; --------------- \n");
        }
    }
    else if (node->token->t_type == ARR_VARIABLE_TOKEN || node->token->t_type == STR_VARIABLE_TOKEN) {
        regs_t reg;
        get_reg(&reg, 8, RAX, VRS_intext(node->token));
        if (!node->child) iprintf(output, "%s %s, %s\n", reg.move, GET_RAW_REG(BASE_BITNESS, RAX), GET_ASMVAR(node));
        else {
            array_info_t arr_info = { .el_size = 1 };
            ARM_get_info((char*)node->token->value, func, &arr_info, ctx->synt->arrs);
            _generate_expression(node->child, output, func, ctx);

            if (arr_info.el_size > 1) iprintf(output, "imul %s, %d\n", GET_RAW_REG(BASE_BITNESS, RAX), arr_info.el_size);
            iprintf(output, "%s %s, %s\n", reg.move, GET_RAW_REG(BASE_BITNESS, RBX), GET_ASMVAR(node));
            iprintf(output, "add %s, %s\n", GET_RAW_REG(BASE_BITNESS, RAX), GET_RAW_REG(BASE_BITNESS, RBX));
            iprintf(
                output, "%s %s,%s[%s]\n", arr_info.el_size <= 2 ? "movzx" : "mov",
                GET_RAW_REG(BASE_BITNESS, RAX), GET_OPERATION_TYPE(arr_info.el_size), GET_RAW_REG(BASE_BITNESS, RAX)
            );
        }
    }
    else if (
        node->token->t_type == BITMOVE_LEFT_TOKEN ||
        node->token->t_type == BITMOVE_RIGHT_TOKEN
    ) {
        _generate_expression(node->child, output, func, ctx);
        iprintf(output, "push %s\n",         GET_RAW_REG(BASE_BITNESS, RAX));
        _generate_expression(node->child->sibling, output, func, ctx);
        iprintf(output, "pop %s\n",          GET_RAW_REG(BASE_BITNESS, RBX));
        iprintf(output, "mov %s, %s\n",      GET_RAW_REG(BASE_BITNESS, RCX), GET_RAW_REG(BASE_BITNESS, RAX));
        if (node->token->t_type == BITMOVE_LEFT_TOKEN) iprintf(output, "shl %s, cl\n", GET_RAW_REG(BASE_BITNESS, RBX));
        else iprintf(output, "shr %s, cl\n", GET_RAW_REG(BASE_BITNESS, RBX));
        iprintf(output, "mov %s, %s\n",      GET_RAW_REG(BASE_BITNESS, RAX), GET_RAW_REG(BASE_BITNESS, RBX));
    }
    else if (
        node->token->t_type == BITAND_TOKEN ||
        node->token->t_type == BITOR_TOKEN ||
        node->token->t_type == BITXOR_TOKEN
    ) {
        _generate_expression(node->child, output, func, ctx);
        iprintf(output, "push %s\n",        GET_RAW_REG(BASE_BITNESS, RAX));
        _generate_expression(node->child->sibling, output, func, ctx);
        iprintf(output, "pop %s\n",         GET_RAW_REG(BASE_BITNESS, RBX));
        if (node->token->t_type == BITAND_TOKEN) iprintf(output, "and %s, %s\n", GET_RAW_REG(BASE_BITNESS, RAX), GET_RAW_REG(BASE_BITNESS, RBX));
        else if (node->token->t_type == BITOR_TOKEN) iprintf(output, "or %s, %s\n", GET_RAW_REG(BASE_BITNESS, RAX), GET_RAW_REG(BASE_BITNESS, RBX));
        else iprintf(output, "xor %s, %s\n", GET_RAW_REG(BASE_BITNESS, RAX), GET_RAW_REG(BASE_BITNESS, RBX));
    }
    else if (
        node->token->t_type == AND_TOKEN ||
        node->token->t_type == OR_TOKEN
    ) {
        int lbl_id = ctx->label++;

        _generate_expression(node->child, output, func, ctx);
        iprintf(output, "cmp %s, 0\n", GET_RAW_REG(BASE_BITNESS, RAX));
        if (node->token->t_type == AND_TOKEN) iprintf(output, "je L_false_%d\n", lbl_id);
        else iprintf(output, "jne L_true_%d\n", lbl_id);

        _generate_expression(node->child->sibling, output, func, ctx);
        iprintf(output, "cmp %s, 0\n", GET_RAW_REG(BASE_BITNESS, RAX));
        iprintf(output, "jne L_true_%d\n", lbl_id);
        iprintf(output, "jmp L_false_%d\n", lbl_id);

        iprintf(output, "L_true_%d:\n", lbl_id);
        iprintf(output, "mov %s, 1\n", GET_RAW_REG(BASE_BITNESS, RAX));
        iprintf(output, "jmp L_end_%d\n", lbl_id);

        iprintf(output, "L_false_%d:\n", lbl_id);
        iprintf(output, "mov %s, 0\n", GET_RAW_REG(BASE_BITNESS, RAX));
        
        iprintf(output, "L_end_%d:\n", lbl_id);
    }
    else if (node->token->t_type == PLUS_TOKEN) {
        _generate_expression(node->child, output, func, ctx);
        iprintf(output, "push %s\n",        GET_RAW_REG(BASE_BITNESS, RAX));
        _generate_expression(node->child->sibling, output, func, ctx);
        iprintf(output, "pop %s\n",         GET_RAW_REG(BASE_BITNESS, RBX));
        iprintf(output, "add %s, %s\n",     GET_RAW_REG(BASE_BITNESS, RAX), GET_RAW_REG(BASE_BITNESS, RBX));
    }
    else if (node->token->t_type == MINUS_TOKEN) {
        _generate_expression(node->child, output, func, ctx);
        iprintf(output, "push %s\n",        GET_RAW_REG(BASE_BITNESS, RAX));
        _generate_expression(node->child->sibling, output, func, ctx);
        iprintf(output, "pop %s\n",         GET_RAW_REG(BASE_BITNESS, RBX));
        iprintf(output, "sub %s, %s\n",     GET_RAW_REG(BASE_BITNESS, RBX), GET_RAW_REG(BASE_BITNESS, RAX));
        iprintf(output, "mov %s, %s\n",     GET_RAW_REG(BASE_BITNESS, RAX), GET_RAW_REG(BASE_BITNESS, RBX));
    }
    else if (node->token->t_type == MULTIPLY_TOKEN) {
        _generate_expression(node->child, output, func, ctx);
        iprintf(output, "push %s\n",        GET_RAW_REG(BASE_BITNESS, RAX));
        _generate_expression(node->child->sibling, output, func, ctx);
        iprintf(output, "pop %s\n",         GET_RAW_REG(BASE_BITNESS, RBX));
        iprintf(output, "imul %s, %s\n",    GET_RAW_REG(BASE_BITNESS, RAX), GET_RAW_REG(BASE_BITNESS, RBX));
    }
    else if (node->token->t_type == DIVIDE_TOKEN) {
        _generate_expression(node->child, output, func, ctx);
        iprintf(output, "push %s\n",        GET_RAW_REG(BASE_BITNESS, RAX));
        _generate_expression(node->child->sibling, output, func, ctx);
        iprintf(output, "mov %s, %s\n",     GET_RAW_REG(BASE_BITNESS, RBX), GET_RAW_REG(BASE_BITNESS, RAX));
        iprintf(output, "pop %s\n",         GET_RAW_REG(BASE_BITNESS, RAX));
        iprintf(output, "cdq\n");
        iprintf(output, "idiv %s\n",        GET_RAW_REG(BASE_BITNESS, RBX));
    }
    else if (node->token->t_type == MODULO_TOKEN) {
        _generate_expression(node->child, output, func, ctx);
        iprintf(output, "push %s\n",        GET_RAW_REG(BASE_BITNESS, RAX));
        _generate_expression(node->child->sibling, output, func, ctx);
        iprintf(output, "mov %s, %s\n",     GET_RAW_REG(BASE_BITNESS, RBX), GET_RAW_REG(BASE_BITNESS, RAX));
        iprintf(output, "pop %s\n",         GET_RAW_REG(BASE_BITNESS, RAX));
        iprintf(output, "cdq\n");
        iprintf(output, "idiv %s\n",        GET_RAW_REG(BASE_BITNESS, RBX));
        iprintf(output, "mov %s, %s\n",     GET_RAW_REG(BASE_BITNESS, RAX), GET_RAW_REG(BASE_BITNESS, RDX));
    }
    else if (
        node->token->t_type == LARGER_TOKEN  ||
        node->token->t_type == LOWER_TOKEN   ||
        node->token->t_type == COMPARE_TOKEN ||
        node->token->t_type == NCOMPARE_TOKEN
    ) {
        _generate_expression(node->child, output, func, ctx);
        iprintf(output, "push %s\n",        GET_RAW_REG(BASE_BITNESS, RAX));
        _generate_expression(node->child->sibling, output, func, ctx);
        iprintf(output, "pop %s\n",         GET_RAW_REG(BASE_BITNESS, RBX));
        iprintf(output, "cmp %s, %s\n",     GET_RAW_REG(BASE_BITNESS, RBX), GET_RAW_REG(BASE_BITNESS, RAX));
        if (node->token->t_type == LARGER_TOKEN)       iprintf(output, "setg al\n");
        else if (node->token->t_type == LOWER_TOKEN)   iprintf(output, "setl al\n");
        else if (node->token->t_type == COMPARE_TOKEN) iprintf(output, "sete al\n");
        else iprintf(output, "setne al\n");
        iprintf(output, "movzx %s, al\n", GET_RAW_REG(BASE_BITNESS, RAX));
    }
    else if (node->token->t_type == CALL_TOKEN) {
        
    }
    else if (node->token->t_type == EXIT_TOKEN) {
        fprintf(output, "\n ; --------------- Exit --------------- \n");
        _generate_expression(node->child, output, func, ctx);
        iprintf(output, "mov %s, %s\n", GET_RAW_REG(BASE_BITNESS, RDI), GET_RAW_REG(BASE_BITNESS, RAX));
        iprintf(output, "mov %s, 60\n", GET_RAW_REG(BASE_BITNESS, RAX));
        iprintf(output, "%s\n", SYSCALL);
    }
    else if (node->token->t_type == RETURN_TOKEN) {
        fprintf(output, "\n ; --------------- Return --------------- \n");
        _generate_expression(node->child, output, func, ctx);
        iprintf(output, "mov %s, %s\n", GET_RAW_REG(BASE_BITNESS, RSP), GET_RAW_REG(BASE_BITNESS, RBP));
        iprintf(output, "pop %s\n",     GET_RAW_REG(BASE_BITNESS, RBP));
        iprintf(output, "ret\n");
    }

    return 1;
}

static int _generate_text_section(ast_node_t* node, FILE* output, gen_ctx_t* ctx) {
    if (!node) return 0;
    for (ast_node_t* child = node->child; child; child = child->sibling) {
        _generate_expression(child, output, NULL, ctx);
    }

    return 1;
}

gen_ctx_t* GEN_create_ctx() {
    gen_ctx_t* ctx = (gen_ctx_t*)mm_malloc(sizeof(gen_ctx_t));
    if (!ctx) return NULL;
    str_memset(ctx, 0, sizeof(gen_ctx_t));
    return ctx;
}

int GEN_destroy_ctx(gen_ctx_t* ctx) {
    if (!ctx) return 0;
    mm_free(ctx);
    return 1;
}

int GEN_generate(gen_ctx_t* ctx, FILE* output) {
    return 1;
}
