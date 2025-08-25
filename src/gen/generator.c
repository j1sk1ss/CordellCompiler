#include <generator.h>

static int _generate_declaration(ast_node_t*, FILE*, const char*, gen_ctx_t*);
static int _generate_assignment( ast_node_t*, FILE*, const char*, gen_ctx_t*);
static int _generate_function(   ast_node_t*, FILE*, const char*, gen_ctx_t*);
static int _generate_syscall(    ast_node_t*, FILE*, const char*, gen_ctx_t*);
static int _generate_switch(     ast_node_t*, FILE*, const char*, gen_ctx_t*);
static int _generate_while(      ast_node_t*, FILE*, const char*, gen_ctx_t*);
static int _generate_if(         ast_node_t*, FILE*, const char*, gen_ctx_t*);

static int _generate_expression(ast_node_t* node, FILE* output, const char* func, gen_ctx_t* ctx) {
    if (!node) return 0;
    if (node->token->t_type == IF_TOKEN)           _generate_if(node, output, func, ctx);
    else if (node->token->t_type == SWITCH_TOKEN)  _generate_switch(node, output, func, ctx);
    else if (node->token->t_type == WHILE_TOKEN)   _generate_while(node, output, func, ctx);
    else if (node->token->t_type == FUNC_TOKEN)    _generate_function(node, output, func, ctx);
    else if (node->token->t_type == SYSCALL_TOKEN) _generate_syscall(node, output, func, ctx);
    else if (node->token->t_type == ASSIGN_TOKEN)   _generate_assignment(node, output, func, ctx);
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

static int _get_variables_size(ast_node_t* head, const char* func, gen_ctx_t* ctx) {
    int size = 0;
    if (!head) return 0;
    for (ast_node_t* expression = head; expression; expression = expression->sibling) {
        if (!expression->token) continue;
        if (!VRS_intext(expression->token)) continue;
        if (expression->token->t_type == ARRAY_TYPE_TOKEN) {
            array_info_t arr_info = { .el_size = 1 };
            if (ARM_get_info(
                (char*)expression->child->sibling->sibling->token->value, 
                func, &arr_info, ctx->synt->arrs
            )) {
                size +=  ALIGN(arr_info.size * arr_info.el_size);
            }
        }
        else if (expression->token->t_type == STR_TYPE_TOKEN) {
            array_info_t arr_info = { .el_size = 1 };
            if (ARM_get_info((char*)expression->child->token->value, func, &arr_info, ctx->synt->arrs)) {
                size += ALIGN(arr_info.size * arr_info.el_size);
            }
        }
        else if (
            expression->token->t_type == SWITCH_TOKEN ||
            expression->token->t_type == WHILE_TOKEN  ||
            expression->token->t_type == IF_TOKEN
        ) size += _get_variables_size(expression->child->sibling->child, func, ctx);
        else if (expression->token->t_type == CASE_TOKEN) size += _get_variables_size(expression->child->child, func, ctx);
        else size += ALIGN(expression->info.offset);
    }

    return size;
}

static int _generate_declaration(ast_node_t* node, FILE* output, const char* func, gen_ctx_t* ctx) {
    ast_node_t* name_node = node->child;
    if (!VRS_intext(name_node->token)) return 0;

    int val = 0;
    int is_const = 0;
    char* derictive = " ";

    if (
        name_node->sibling->token->t_type != UNKNOWN_NUMERIC_TOKEN && 
        name_node->sibling->token->t_type != CHAR_VALUE_TOKEN
    ) {
        is_const = 0;
        _generate_expression(name_node->sibling, output, func, ctx);
    }
    else {
        is_const = 1;
        switch (VRS_variable_bitness(name_node->token, 1)) {
            default:
            case 64: derictive = " qword "; break;
            case 32: derictive = " dword "; break;
            case 16: derictive = " word ";  break;
            case 8: derictive  = " byte ";  break;
        }

        if (name_node->sibling->token->t_type == UNKNOWN_NUMERIC_TOKEN) val = str_atoi((char*)name_node->sibling->token->value);
        else if (name_node->sibling->token->t_type == CHAR_VALUE_TOKEN) val = *name_node->sibling->token->value;
    }

    char source[36] = { 0 };
    if (is_const) sprintf(source, "%d", val); /* If this is constant value, we can store it in stack */
    else sprintf(source, "%s", GET_RAW_REG(BASE_BITNESS, RAX));

    iprintf(
        output, "mov%s%s, %s ; decl %s = %s\n", 
        derictive, GET_ASMVAR(name_node), source, (char*)name_node->token->value, source
    );

    return 1;
}

static int _generate_assignment(ast_node_t* node, FILE* output, const char* func, gen_ctx_t* ctx) {
    ast_node_t* left  = node->child;
    ast_node_t* right = left->sibling;

    fprintf(output, "\n; --------------- Assignment: %s = %s --------------- \n", left->token->value, right->token->value);

    /*
    We store right result to RAX, and move it to stack with offset of left.
    Pointer assignment. Also we check if this variable is ptr, array or etc.
    Markers are 64 bits size and first child.
    */
    if ((VRS_variable_bitness(left->token, 1) == BASE_BITNESS) && left->child) {
        /* If left node is array or string (array too) with elem size info. */
        array_info_t arr_info = { .el_size = 1 };
        int is_ptr = (
            ARM_get_info((char*)node->child->token->value, func, &arr_info, ctx->synt->arrs) && 
            VRS_intext(node->child->token)
        );

        /*
        Generate offset movement in this array-like data type.
        Then multiply it by arr el_size.
        */
        _generate_expression(left->child, output, func, ctx);
        if (arr_info.el_size > 1) iprintf(output, "imul %s, %d\n", GET_RAW_REG(BASE_BITNESS, RAX), arr_info.el_size);
        iprintf(output, "%s %s, %s\n", !is_ptr ? "mov" : "lea", GET_REG(node, 1), GET_ASMVAR(left));

        iprintf(output, "add %s, %s\n", GET_RAW_REG(BASE_BITNESS, RAX), GET_RAW_REG(BASE_BITNESS, RBX));
        iprintf(output, "push %s\n", GET_RAW_REG(BASE_BITNESS, RAX));

        _generate_expression(right, output, func, ctx);
        iprintf(output, "pop %s\n", GET_RAW_REG(BASE_BITNESS, RBX));

        regs_t reg;
        get_reg(&reg, arr_info.el_size, RAX, 0);
        iprintf(output, "mov%s[%s], %s\n", reg.operation, GET_RAW_REG(BASE_BITNESS, RBX), reg.name);
    }
    else {
        _generate_expression(right, output, func, ctx);
        iprintf(output, "mov %s, %s\n", GET_ASMVAR(left), GET_RAW_REG(BASE_BITNESS, RAX));
    }

    iprint_line(output);
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
    ast_node_t* program_body = ctx->synt->r->child;
    ast_node_t* prefix_node  = program_body;
    ast_node_t* main_node    = prefix_node->sibling;

    /*
    Generate data section. Here we store static arrays,
    static strings and etc.
    Also we store here global vars.
    */
    fprintf(output, "\nsection .data\n");
    _generate_data_section(prefix_node, output, DATA_SECTION, _generate_init);
    _generate_data_section(main_node, output, DATA_SECTION, _generate_init);

    /*
    Generate rodata section. Here we store strings, that
    not assign to any variable.
    */
    fprintf(output, "\nsection .rodata\n");
    _generate_data_section(prefix_node, output, RODATA_SECTION, _generate_init);
    _generate_data_section(main_node, output, RODATA_SECTION, _generate_init);

    /* Generate .bss section for not pre-init arrays. */
    fprintf(output, "\nsection .bss\n");
    _generate_data_section(prefix_node, output, DATA_SECTION, _generate_raw);
    _generate_data_section(main_node, output, DATA_SECTION, _generate_raw);

    fprintf(output, "\nsection .text\n");
    /* Generate prefix section code */
    if (prefix_node) {
        for (ast_node_t* child = prefix_node->child; child; child = child->sibling) {
            if (!child->token) continue;
            switch (child->token->t_type) {
                case IMPORT_SELECT_TOKEN:
                    for (ast_node_t* func = child->child->child; func; func = func->sibling)
                        fprintf(output, "extern __%s__\n", func->token->value);
                break;

                case FUNC_TOKEN:
                    fprintf(output, "global __%s__\n", child->child->token->value);
                break;
                default: break;
            }
        }

        _generate_text_section(prefix_node, output, ctx);
    }

    /* Generate main section code */
    if (main_node) {
        fprintf(output, "\nglobal _start\n\n");
        fprintf(output, "_start:\n");

        /* Save old stack and reserve new one */
        iprintf(output, "push %s\n", GET_RAW_REG(BASE_BITNESS, RBP));
        iprintf(output, "mov %s, %s\n", GET_RAW_REG(BASE_BITNESS, RBP), GET_RAW_REG(BASE_BITNESS, RSP));
        iprintf(output, "sub %s, %d\n", GET_RAW_REG(BASE_BITNESS, RSP), ALIGN(_get_variables_size(main_node->child, NULL, ctx)));
       
        _generate_text_section(main_node, output, ctx);
    }

    return 1;
}
