#include <generator.h>

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

static int _generate_declaration(tree_t*, FILE*, const char*, gen_ctx_t*);
static int _generate_assignment( tree_t*, FILE*, const char*, gen_ctx_t*);
static int _generate_function(   tree_t*, FILE*, const char*, gen_ctx_t*);
static int _generate_syscall(    tree_t*, FILE*, const char*, gen_ctx_t*);
static int _generate_switch(     tree_t*, FILE*, const char*, gen_ctx_t*);
static int _generate_while(      tree_t*, FILE*, const char*, gen_ctx_t*);
static int _generate_if(         tree_t*, FILE*, const char*, gen_ctx_t*);

static int _generate_raw(token_type_t t_type, tree_t* entry, FILE* output) {
    if (!entry->first_child) return 0;
    switch (t_type) {
        case LONG_TYPE_TOKEN:  iprintf(output, "__%s__: resq 1\n", (char*)entry->first_child->token->value); break;
        case INT_TYPE_TOKEN:   iprintf(output, "__%s__: resd 1\n", (char*)entry->first_child->token->value); break;
        case SHORT_TYPE_TOKEN: iprintf(output, "__%s__: resw 1\n", (char*)entry->first_child->token->value); break;
        case CHAR_TYPE_TOKEN:  iprintf(output, "__%s__: resb 1\n", (char*)entry->first_child->token->value); break;
        case ARRAY_TYPE_TOKEN: {
            tree_t* size   = entry->first_child;
            tree_t* t_type = size->next_sibling;
            tree_t* name   = t_type->next_sibling;
            if (!name->next_sibling) {
                char* directive = "resb";
                if (t_type->token->t_type == SHORT_TYPE_TOKEN)     directive = "resw";
                else if (t_type->token->t_type == INT_TYPE_TOKEN)  directive = "resd";
                else if (t_type->token->t_type == LONG_TYPE_TOKEN) directive = "resq";
                iprintf(output, "__%s__: %s %s\n", name->token->value, directive, size->token->value);
            }
        }
        break;
        default: break;
    }

    return 1;
}

static int _generate_init(token_type_t t_type, tree_t* entry, FILE* output) {
    switch (t_type) {
        case STR_TYPE_TOKEN:   iprintf(output, "__%s__ db '%s', 0\n", (char*)entry->first_child->token->value, (char*)entry->first_child->next_sibling->token->value); break;
        case LONG_TYPE_TOKEN:  iprintf(output, "__%s__ dq %s\n", (char*)entry->first_child->token->value, (char*)entry->first_child->next_sibling->token->value); break;
        case INT_TYPE_TOKEN:   iprintf(output, "__%s__ dd %s\n", (char*)entry->first_child->token->value, (char*)entry->first_child->next_sibling->token->value); break;
        case SHORT_TYPE_TOKEN: iprintf(output, "__%s__ dw %s\n", (char*)entry->first_child->token->value, (char*)entry->first_child->next_sibling->token->value); break;
        case CHAR_TYPE_TOKEN:  iprintf(output, "__%s__ db %s\n", (char*)entry->first_child->token->value, (char*)entry->first_child->next_sibling->token->value); break;
        case ARRAY_TYPE_TOKEN: {
            tree_t* size   = entry->first_child;
            tree_t* t_type = size->next_sibling;
            tree_t* name   = t_type->next_sibling;
            if (name->next_sibling) {
                char* directive = "db";
                if (t_type->token->t_type == SHORT_TYPE_TOKEN)     directive = "dw";
                else if (t_type->token->t_type == INT_TYPE_TOKEN)  directive = "dd";
                else if (t_type->token->t_type == LONG_TYPE_TOKEN) directive = "dq";
                iprintf(output, "%s %s ", name->token->value, directive);
                for (tree_t* elem = name->next_sibling; elem; elem = elem->next_sibling) {
                    if (elem->token->t_type == UNKNOWN_NUMERIC_TOKEN) fprintf(output, "%s%s", elem->token->value, elem->next_sibling ? "," : "\n");
                    else {
                        int value = 0;
                        char* token_start = (char*)elem->token->value;
                        while (*token_start) {
                            value += *token_start;
                            token_start++;
                        }

                        fprintf(output, "%i%s", value, elem->next_sibling ? "," : "\n");
                    }
                }
            }

            break;
        }
        default: break;
    }

    return 1;
}

/*
section 1 is a data section
section 2 is a rodata section
*/
#define DATA_SECTION   1
#define RODATA_SECTION 2
static int _generate_data_section(
    tree_t* node, FILE* output, int section, int (*data_gen)(token_type_t, tree_t*, FILE*)
) {
    if (!node) return 0;
    for (tree_t* child = node->first_child; child; child = child->next_sibling) {
        if (!child->token) {
            _generate_data_section(child, output, section, data_gen);
            continue;
        }

        if (
            (section == DATA_SECTION && child->token->glob) || (section == RODATA_SECTION && child->token->ro)
        ) data_gen(child->token->t_type, child, output);
        else if (VRS_intext(child->token)) {
            switch (child->token->t_type) {
                case IF_TOKEN:
                case SYSCALL_TOKEN:
                case WHILE_TOKEN:  _generate_data_section(child, output, section, data_gen);                                          break;
                case SWITCH_TOKEN: _generate_data_section(child->first_child->next_sibling, output, section, data_gen);               break;
                case DEFAULT_TOKEN:
                case CASE_TOKEN:   _generate_data_section(child->first_child->first_child, output, section, data_gen);                break;
                case FUNC_TOKEN:   _generate_data_section(child->first_child->next_sibling->next_sibling, output, section, data_gen); break;
                default: break;
            }
        }
    }

    return 1;
}

static int _generate_expression(tree_t* node, FILE* output, const char* func, gen_ctx_t* ctx) {
    if (!node) return 0;
    if (node->token->t_type == IF_TOKEN)           _generate_if(node, output, func, ctx);
    else if (node->token->t_type == SWITCH_TOKEN)  _generate_switch(node, output, func, ctx);
    else if (node->token->t_type == WHILE_TOKEN)   _generate_while(node, output, func, ctx);
    else if (node->token->t_type == FUNC_TOKEN)    _generate_function(node, output, func, ctx);
    else if (node->token->t_type == SYSCALL_TOKEN) _generate_syscall(node, output, func, ctx);
    else if (node->token->t_type == ASIGN_TOKEN)   _generate_assignment(node, output, func, ctx);
    else if (node->token->t_type == UNKNOWN_NUMERIC_TOKEN) iprintf(output, "mov %s, %s\n", GET_RAW_REG(BASE_BITNESS, RAX), node->token->value);
    else if (node->token->t_type == CHAR_VALUE_TOKEN)      iprintf(output, "mov %s, %i\n", GET_RAW_REG(8, RAX), *node->token->value);
    else if (node->token->ptr && VRS_isdecl(node->token->t_type)) _generate_declaration(node, output, func, ctx);
    else if (node->token->ptr && !VRS_isdecl(node->token->t_type) && VRS_isptr(node->token)) {
        if (!node->first_child) iprintf(output, "mov %s, %s\n", GET_RAW_REG(BASE_BITNESS, RAX), GET_ASMVAR(node));
        else {
            variable_info_t info;
            if (VRM_get_info((char*)node->token->value, func, &info, ctx->synt->vars)) {
                _generate_expression(node->first_child, output, func, ctx);

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

        _generate_expression(node->first_child, output, func, ctx);
    }
    else if (node->token->t_type == ARRAY_TYPE_TOKEN) {
        if (node->first_child && VRS_intext(node->token)) {
            tree_t* size   = node->first_child;
            tree_t* t_type = size->next_sibling;
            tree_t* name   = t_type->next_sibling;

            array_info_t arr_info = { .el_size = 1 };
            if (ARM_get_info((char*)name->token->value, func, &arr_info, ctx->synt->arrs)) {
                tree_t* vals = name->next_sibling;
                if (vals && vals->token->t_type != DELIMITER_TOKEN) {
                    fprintf(output, "\n ; --------------- Array setup %s --------------- \n", name->token->value);

                    regs_t reg;
                    get_reg(&reg, arr_info.el_size, RAX, 0);

                    int base_off = node->variable_offset;
                    for (tree_t* v = vals; v && v->token->t_type != DELIMITER_TOKEN; v = v->next_sibling) {
                        if (v->token->t_type == UNKNOWN_NUMERIC_TOKEN) iprintf(
                            output, "mov%s[%s - %d], %d\n", reg.operation, GET_RAW_REG(BASE_BITNESS, RBP), base_off, str_atoi((char*)v->token->value)
                        );
                        else if (v->token->t_type == CHAR_VALUE_TOKEN) iprintf(
                            output, "mov%s[%s - %d], %d\n", reg.operation, GET_RAW_REG(BASE_BITNESS, RBP), base_off, *v->token->value
                        );
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
        tree_t* name_node = node->first_child;
        if (name_node->next_sibling && VRS_intext(name_node->token)) {
            fprintf(output, "\n ; --------------- String setup %s --------------- \n", name_node->token->value);
            tree_t* val_node = name_node->next_sibling;
            char* val_head = (char*)val_node->token->value;
            int base_off = name_node->variable_offset;
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
        if (!node->first_child) iprintf(output, "%s %s, %s\n", reg.move, GET_RAW_REG(BASE_BITNESS, RAX), GET_ASMVAR(node));
        else {
            array_info_t arr_info = { .el_size = 1 };
            ARM_get_info((char*)node->token->value, func, &arr_info, ctx->synt->arrs);
            _generate_expression(node->first_child, output, func, ctx);

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
        _generate_expression(node->first_child, output, func, ctx);
        iprintf(output, "push %s\n",         GET_RAW_REG(BASE_BITNESS, RAX));
        _generate_expression(node->first_child->next_sibling, output, func, ctx);
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
        _generate_expression(node->first_child, output, func, ctx);
        iprintf(output, "push %s\n",        GET_RAW_REG(BASE_BITNESS, RAX));
        _generate_expression(node->first_child->next_sibling, output, func, ctx);
        iprintf(output, "pop %s\n",         GET_RAW_REG(BASE_BITNESS, RBX));
        if (node->token->t_type == BITAND_TOKEN) iprintf(output, "and %s, %s\n", GET_RAW_REG(BASE_BITNESS, RAX), GET_RAW_REG(BASE_BITNESS, RBX));
        else if (node->token->t_type == BITOR_TOKEN) iprintf(output, "or %s, %s\n", GET_RAW_REG(BASE_BITNESS, RAX), GET_RAW_REG(BASE_BITNESS, RBX));
        else iprintf(output, "xor %s, %s\n", GET_RAW_REG(BASE_BITNESS, RAX), GET_RAW_REG(BASE_BITNESS, RBX));
    }
    else if (
        node->token->t_type == AND_TOKEN ||
        node->token->t_type == OR_TOKEN
    ) {
        _generate_expression(node->first_child, output, func, ctx);
        iprintf(output, "push %s\n",        GET_RAW_REG(BASE_BITNESS, RAX));
        _generate_expression(node->first_child->next_sibling, output, func, ctx);
        iprintf(output, "mov %s, %s\n",     GET_RAW_REG(BASE_BITNESS, RBX), GET_RAW_REG(BASE_BITNESS, RAX));
        iprintf(output, "pop %s\n",         GET_RAW_REG(BASE_BITNESS, RAX));
        if (node->token->t_type == AND_TOKEN) iprintf(output, "and %s, %s\n", GET_RAW_REG(BASE_BITNESS, RAX), GET_RAW_REG(BASE_BITNESS, RBX));
        else iprintf(output, "or %s, %s\n", GET_RAW_REG(BASE_BITNESS, RAX), GET_RAW_REG(BASE_BITNESS, RBX));
    }
    else if (node->token->t_type == PLUS_TOKEN) {
        _generate_expression(node->first_child, output, func, ctx);
        iprintf(output, "push %s\n",        GET_RAW_REG(BASE_BITNESS, RAX));
        _generate_expression(node->first_child->next_sibling, output, func, ctx);
        iprintf(output, "pop %s\n",         GET_RAW_REG(BASE_BITNESS, RBX));
        iprintf(output, "add %s, %s\n",     GET_RAW_REG(BASE_BITNESS, RAX), GET_RAW_REG(BASE_BITNESS, RBX));
    }
    else if (node->token->t_type == MINUS_TOKEN) {
        _generate_expression(node->first_child, output, func, ctx);
        iprintf(output, "push %s\n",        GET_RAW_REG(BASE_BITNESS, RAX));
        _generate_expression(node->first_child->next_sibling, output, func, ctx);
        iprintf(output, "pop %s\n",         GET_RAW_REG(BASE_BITNESS, RBX));
        iprintf(output, "sub %s, %s\n",     GET_RAW_REG(BASE_BITNESS, RBX), GET_RAW_REG(BASE_BITNESS, RAX));
        iprintf(output, "mov %s, %s\n",     GET_RAW_REG(BASE_BITNESS, RAX), GET_RAW_REG(BASE_BITNESS, RBX));
    }
    else if (node->token->t_type == MULTIPLY_TOKEN) {
        _generate_expression(node->first_child, output, func, ctx);
        iprintf(output, "push %s\n",        GET_RAW_REG(BASE_BITNESS, RAX));
        _generate_expression(node->first_child->next_sibling, output, func, ctx);
        iprintf(output, "pop %s\n",         GET_RAW_REG(BASE_BITNESS, RBX));
        iprintf(output, "imul %s, %s\n",    GET_RAW_REG(BASE_BITNESS, RAX), GET_RAW_REG(BASE_BITNESS, RBX));
    }
    else if (node->token->t_type == DIVIDE_TOKEN) {
        _generate_expression(node->first_child, output, func, ctx);
        iprintf(output, "push %s\n",        GET_RAW_REG(BASE_BITNESS, RAX));
        _generate_expression(node->first_child->next_sibling, output, func, ctx);
        iprintf(output, "mov %s, %s\n",     GET_RAW_REG(BASE_BITNESS, RBX), GET_RAW_REG(BASE_BITNESS, RAX));
        iprintf(output, "pop %s\n",         GET_RAW_REG(BASE_BITNESS, RAX));
        iprintf(output, "cdq\n");
        iprintf(output, "idiv %s\n",        GET_RAW_REG(BASE_BITNESS, RBX));
    }
    else if (node->token->t_type == MODULO_TOKEN) {
        _generate_expression(node->first_child, output, func, ctx);
        iprintf(output, "push %s\n",        GET_RAW_REG(BASE_BITNESS, RAX));
        _generate_expression(node->first_child->next_sibling, output, func, ctx);
        iprintf(output, "mov %s, %s\n",     GET_RAW_REG(BASE_BITNESS, RBX), GET_RAW_REG(BASE_BITNESS, RAX));
        iprintf(output, "pop %s\n",         GET_RAW_REG(BASE_BITNESS, RAX));
        iprintf(output, "cdq\n");
        iprintf(output, "idiv %s\n",        GET_RAW_REG(BASE_BITNESS, RBX));
        iprintf(output, "mov %s, %s\n",     GET_RAW_REG(BASE_BITNESS, RAX), GET_RAW_REG(BASE_BITNESS, RDX));
    }
    else if (
        node->token->t_type == LARGER_TOKEN ||
        node->token->t_type == LOWER_TOKEN ||
        node->token->t_type == COMPARE_TOKEN ||
        node->token->t_type == NCOMPARE_TOKEN
    ) {
        _generate_expression(node->first_child, output, func, ctx);
        iprintf(output, "push %s\n",        GET_RAW_REG(BASE_BITNESS, RAX));
        _generate_expression(node->first_child->next_sibling, output, func, ctx);
        iprintf(output, "pop %s\n",         GET_RAW_REG(BASE_BITNESS, RBX));
        iprintf(output, "cmp %s, %s\n",     GET_RAW_REG(BASE_BITNESS, RBX), GET_RAW_REG(BASE_BITNESS, RAX));
        if (node->token->t_type == LARGER_TOKEN)       iprintf(output, "setg al\n");
        else if (node->token->t_type == LOWER_TOKEN)   iprintf(output, "setl al\n");
        else if (node->token->t_type == COMPARE_TOKEN) iprintf(output, "sete al\n");
        else iprintf(output, "setne al\n");
        iprintf(output, "movzx %s, al\n", GET_RAW_REG(BASE_BITNESS, RAX));
    }
    else if (node->token->t_type == CALL_TOKEN) {
        /*
        Generating function preparations.
        1) Getting function name and args.
        2) Put args to the stack.
        */
        int variables_size = 0;
        tree_t* func_name_node = node;
        tree_t* args_node = func_name_node->first_child;

        /*
        Saving params in stack.
        */
        int arg_count = 0;
        tree_t* args[128] = { NULL };
        for (tree_t* arg = args_node->first_child; arg; arg = arg->next_sibling) {
            args[arg_count++] = arg;
        }

        fprintf(output, "\n ; --------------- Call function %s --------------- \n", func_name_node->token->value);
        int pushed_args = 0;

#if (BASE_BITNESS == BIT64)
        static const int args_regs[] = { RDI, RSI, RDX, RCX, R8, R9 };
        for (pushed_args = 0; pushed_args < MIN(arg_count, 6); pushed_args++) {
            _generate_expression(args[pushed_args], output, func, ctx);
            iprintf(output, "mov %s, %s \n", GET_RAW_REG(BASE_BITNESS, args_regs[pushed_args]), GET_RAW_REG(BASE_BITNESS, RAX));
        }
#endif

        int stack_args = arg_count - pushed_args - 1;
        while (stack_args >= 0) {
            _generate_expression(args[stack_args--], output, func, ctx);
            iprintf(output, "push %s\n", GET_RAW_REG(BASE_BITNESS, RAX));
        }

        iprintf(output, "call __%s__\n", func_name_node->token->value);
        if (stack_args > 0) iprintf(output, "add %s, %d\n", GET_RAW_REG(BASE_BITNESS, RSP), stack_args * 8);
        fprintf(output, " ; --------------- \n");
    }
    else if (node->token->t_type == EXIT_TOKEN) {
        fprintf(output, "\n ; --------------- Exit --------------- \n");
        _generate_expression(node->first_child, output, func, ctx);
        iprintf(output, "mov %s, %s\n", GET_RAW_REG(BASE_BITNESS, RDI), GET_RAW_REG(BASE_BITNESS, RAX));
        iprintf(output, "mov %s, 60\n", GET_RAW_REG(BASE_BITNESS, RAX));
        iprintf(output, "%s\n", SYSCALL);
    }
    else if (node->token->t_type == RETURN_TOKEN) {
        fprintf(output, "\n ; --------------- Return --------------- \n");
        _generate_expression(node->first_child, output, func, ctx);
        iprintf(output, "mov %s, %s\n", GET_RAW_REG(BASE_BITNESS, RSP), GET_RAW_REG(BASE_BITNESS, RBP));
        iprintf(output, "pop %s\n",     GET_RAW_REG(BASE_BITNESS, RBP));
        iprintf(output, "ret\n");
    }

    return 1;
}

static int _get_variables_size(tree_t* head, const char* func, gen_ctx_t* ctx) {
    int size = 0;
    if (!head) return 0;
    for (tree_t* expression = head; expression; expression = expression->next_sibling) {
        if (!expression->token) continue;
        if (!VRS_intext(expression->token)) continue;
        if (expression->token->t_type == ARRAY_TYPE_TOKEN) {
            array_info_t arr_info = { .el_size = 1 };
            if (ARM_get_info(
                (char*)expression->first_child->next_sibling->next_sibling->token->value, 
                func, &arr_info, ctx->synt->arrs
            )) {
                size +=  ALIGN(arr_info.size * arr_info.el_size);
            }
        }
        else if (expression->token->t_type == STR_TYPE_TOKEN) {
            array_info_t arr_info = { .el_size = 1 };
            if (ARM_get_info((char*)expression->first_child->token->value, func, &arr_info, ctx->synt->arrs)) {
                size += ALIGN(arr_info.size * arr_info.el_size);
            }
        }
        else if (
            expression->token->t_type == SWITCH_TOKEN ||
            expression->token->t_type == WHILE_TOKEN ||
            expression->token->t_type == IF_TOKEN
        ) size += _get_variables_size(expression->first_child->next_sibling->first_child, func, ctx);
        else if (expression->token->t_type == CASE_TOKEN) size += _get_variables_size(expression->first_child->first_child, func, ctx);
        else size += ALIGN(expression->variable_size);
    }

    return size;
}

static int _generate_declaration(tree_t* node, FILE* output, const char* func, gen_ctx_t* ctx) {
    int val = 0;
    int type = 0;

    tree_t* name_node = node->first_child;
    if (!VRS_intext(name_node->token)) return 0;

    char* derictive = " ";
    if (
        name_node->next_sibling->token->t_type != UNKNOWN_NUMERIC_TOKEN && 
        name_node->next_sibling->token->t_type != CHAR_VALUE_TOKEN
    ) {
        type = 0;
        _generate_expression(name_node->next_sibling, output, func, ctx);
    }
    else {
        type = 1;
        switch (VRS_variable_bitness(name_node->token, 1)) {
            default:
            case 64: derictive = " qword "; break;
            case 32: derictive = " dword "; break;
            case 16: derictive = " word ";  break;
            case 8: derictive  = " byte ";  break;
        }

        if (name_node->next_sibling->token->t_type == UNKNOWN_NUMERIC_TOKEN) val = str_atoi((char*)name_node->next_sibling->token->value);
        else if (name_node->next_sibling->token->t_type == CHAR_VALUE_TOKEN) val = *name_node->next_sibling->token->value;
    }

    char source[36] = { 0 };
    if (type) sprintf(source, "%d", val);
    else sprintf(source, "%s", GET_RAW_REG(BASE_BITNESS, RAX));

    iprintf(output, "mov%s%s, %s ; decl %s = %s\n", derictive, GET_ASMVAR(name_node), source, (char*)name_node->token->value, source);
    return 1;
}

static int _generate_function(tree_t* node, FILE* output, const char* func, gen_ctx_t* ctx) {
    tree_t* name_node   = node->first_child;
    tree_t* params_node = name_node->next_sibling;
    tree_t* body_node   = params_node->next_sibling;

    fprintf(output, "\n ; --------------- Function %s --------------- \n", name_node->token->value);
    iprintf(output, "jmp __end_%s__\n", name_node->token->value);
    iprintf(output, "__%s__:\n", name_node->token->value);

    iprintf(output, "push %s\n", GET_RAW_REG(BASE_BITNESS, RBP));
    iprintf(output, "mov %s, %s\n", GET_RAW_REG(BASE_BITNESS, RBP), GET_RAW_REG(BASE_BITNESS, RSP));

    /*
    Reserving stack memory for local variables. (Creating stack frame).
    We should go into function body and find all local variables.
    Also we remember input variables.
    */
    int local_vars_size = _get_variables_size(
        params_node->first_child, (char*)name_node->token->value, ctx
    ) + _get_variables_size(
        body_node->first_child, (char*)name_node->token->value, ctx
    );

    iprintf(output, "sub %s, %d\n", GET_RAW_REG(BASE_BITNESS, RSP), ALIGN(local_vars_size));

    /*
    Loading input args to stack.
    */
    int pop_params = 0;
    int stack_offset = 8;

    static const int args_regs[] = { RDI, RSI, RDX, RCX, R8, R9 };
    for (tree_t* param = params_node->first_child; param; param = param->next_sibling) {
        int param_size = param->variable_size;
        char* param_name = (char*)param->first_child->token->value;

        regs_t reg;
#if (BASE_BITNESS == BIT64)
        get_reg(&reg, VRS_variable_bitness(param->first_child->token, 1) / 8, args_regs[pop_params], 0);
        if (pop_params < 6) iprintf(output, "mov%s[%s - %d], %s\n", reg.operation, GET_RAW_REG(BASE_BITNESS, RBP), param->first_child->variable_offset, reg.name);
        else
#else
        get_reg(&reg, VRS_variable_bitness(param->first_child->token, 1) / 8, RAX, 0);
#endif
        {
            iprintf(output, "mov %s, [%s + %d] ; int64 %s \n", reg.name, GET_RAW_REG(BASE_BITNESS, RBP), stack_offset, param_name);
            iprintf(output, "mov%s[%s - %d], %s\n", reg.operation, GET_RAW_REG(BASE_BITNESS, RBP), param->first_child->variable_offset, reg.name);
            stack_offset += param_size;
        }

        pop_params++;
    }

    /*
    Function body without return statement.
    All expressions will use one shared register EAX.
    */
    for (tree_t* part = body_node->first_child; part; part = part->next_sibling) {
        _generate_expression(part, output, (char*)name_node->token->value, ctx);
    }

    fprintf(output, " ; --------------- \n");
    iprintf(output, "__end_%s__:\n", name_node->token->value);
    return 1;
}

static int _generate_while(tree_t* node, FILE* output, const char* func, gen_ctx_t* ctx) {
    int current_label = ctx->label++;
    tree_t* condition = node->first_child;
    tree_t* body = condition->next_sibling->first_child;

    fprintf(output, "\n ; --------------- while cycle [%i] --------------- \n", current_label);
    iprintf(output, "__while_%d__:\n", current_label);

    _generate_expression(condition, output, func, ctx);
    iprintf(output, "cmp rax, 0\n");
    iprintf(output, "je __end_while_%d__\n", current_label);

    while (body) {
        _generate_expression(body, output, func, ctx);
        body = body->next_sibling;
    }

    iprintf(output, "jmp __while_%d__\n", current_label);

    fprintf(output, " ; --------------- \n");
    iprintf(output, "__end_while_%d__:\n", current_label);
    return 1;
}

static int _cmp(const void* a, const void* b) {
    return (*(int*)a - *(int*)b);
}

static int _generate_case_binary_jump(FILE* output, int* values, int left, int right, int label_id, int default_scope) {
    if (left > right) {
        if (default_scope) iprintf(output, "jmp __default_%d__\n", label_id);
        else iprintf(output, "jmp __end_switch_%d__\n", label_id);
        return 0;
    }

    int mid = (left + right) / 2;
    int val = values[mid];

    iprintf(output, "cmp %s, %d\n", GET_RAW_REG(BASE_BITNESS, RAX), val);
    iprintf(output, "jl __case_l_%d_%d__\n", val, label_id);
    iprintf(output, "jg __case_r_%d_%d__\n", val, label_id);
    iprintf(output, "jmp __case_%d_%d__\n", val, label_id);

    iprintf(output, "__case_l_%d_%d__:\n", val, label_id);
    _generate_case_binary_jump(output, values, left, mid - 1, label_id, default_scope);

    iprintf(output, "__case_r_%d_%d__:\n", val, label_id);
    _generate_case_binary_jump(output, values, mid + 1, right, label_id, default_scope);
    return 1;
}

static int _generate_switch(tree_t* node, FILE* output, const char* func, gen_ctx_t* ctx) {
    int current_label = ctx->label++;
    tree_t* stmt = node->first_child;
    tree_t* cases = stmt->next_sibling;

    int cases_count = 0;
    int values[128] = { -1 };

    fprintf(output, "\n ; --------------- switch [%i] --------------- \n", current_label);

    int have_default = 0;
    iprintf(output, "jmp __end_cases_%d__\n", current_label);
    for (tree_t* curr_case = cases->first_child; curr_case; curr_case = curr_case->next_sibling) {
        if (curr_case->token->t_type == DEFAULT_TOKEN) {
            have_default = 1;
            iprintf(output, "__default_%d__:\n", current_label);
        }
        else {
            int case_value = str_atoi((char*)curr_case->token->value);
            iprintf(output, "__case_%d_%d__:\n", case_value, current_label);
            values[cases_count++] = case_value;
        }

        for (tree_t* part = curr_case->first_child->first_child; part; part = part->next_sibling) {
            _generate_expression(part, output, func, ctx);
        }

        iprintf(output, "jmp __end_switch_%d__\n", current_label);
    }

    sort_qsort(values, cases_count, sizeof(int), _cmp);
    iprintf(output, "__end_cases_%d__:\n", current_label);
    _generate_expression(stmt, output, func, ctx);
    _generate_case_binary_jump(output, values, 0, cases_count - 1, current_label, have_default);

    iprintf(output, "__end_switch_%d__:\n", current_label);
    fprintf(output, " ; --------------- \n");
    return 1;
}

static int _generate_if(tree_t* node, FILE* output, const char* func, gen_ctx_t* ctx) {
    int current_label = ctx->label++;
    tree_t* condition = node->first_child;
    tree_t* body = condition->next_sibling;
    tree_t* else_body = body->next_sibling;

    fprintf(output, "\n ; --------------- if statement [%i] --------------- \n", current_label);
    _generate_expression(condition, output, func, ctx);
    iprintf(output, "cmp %s, 0\n", GET_RAW_REG(BASE_BITNESS, RAX));
    if (else_body) iprintf(output, "je __else_%d__\n", current_label);
    else iprintf(output, "je __end_if_%d__\n", current_label);

    tree_t* body_exp = body->first_child;
    while (body_exp) {
        _generate_expression(body_exp, output, func, ctx);
        body_exp = body_exp->next_sibling;
    }

    iprintf(output, "jmp __end_if_%d__\n", current_label);

    if (else_body) {
        iprintf(output, "__else_%d__:\n", current_label);
        tree_t* else_body_exp = else_body->first_child;
        while (else_body_exp) {
            _generate_expression(else_body_exp, output, func, ctx);
            else_body_exp = else_body_exp->next_sibling;
        }
    }

    fprintf(output, " ; --------------- \n");
    iprintf(output, "__end_if_%d__:\n", current_label);
    return 1;
}

/* https://blog.rchapman.org/posts/Linux_System_Call_Table_for_x86_64/ */
/* https://gist.github.com/GabriOliv/a9411fa771a1e5d94105cb05cbaebd21 */
/* https://math.hws.edu/eck/cs220/f22/registers.html */
static int _generate_syscall(tree_t* node, FILE* output, const char* func, gen_ctx_t* ctx) {
    static const int args_regs[] = { 
#if (BASE_BITNESS == BIT64)
        RAX, RDI, RSI, RDX, R10, R8, R9
#else
        RAX, RBX, RCX, RDX, RSI, RDI, RBP
#endif
    };

    fprintf(output, "\n ; --------------- system call --------------- \n");

    int arg_index = 0;
    tree_t* args = node->first_child;
    while (args) {
        regs_t reg;
        int is_ptr = (
            ARM_get_info((char*)args->token->value, func, NULL, ctx->synt->arrs) && 
            !VRS_intext(args->token)
        );
        
        get_reg(&reg, VRS_variable_bitness(args->token, 1) / 8, args_regs[arg_index++], is_ptr);
        iprintf(output, "%s%s%s, %s\n", reg.move, reg.operation, reg.name, GET_ASMVAR(args));
        args = args->next_sibling;
    }

    iprintf(output, "%s\n", SYSCALL);
    fprintf(output, " ; --------------- \n");

    return 1;
}

static int _generate_assignment(tree_t* node, FILE* output, const char* func, gen_ctx_t* ctx) {
    tree_t* left  = node->first_child;
    tree_t* right = left->next_sibling;

    fprintf(output, "\n; --------------- Assignment: %s = %s --------------- \n", left->token->value, right->token->value);

    /*
    We store right result to RAX, and move it to stack with offset of left.
    Pointer assignment. Also we check if this variable is ptr, array or etc.
    Markers are 64 bits size and first child.
    */
    if ((VRS_variable_bitness(left->token, 1) == BASE_BITNESS) && left->first_child) {
        /* If left node is array or string (array too) with elem size info. */
        array_info_t arr_info = { .el_size = 1 };
        int is_ptr = (
            ARM_get_info((char*)node->first_child->token->value, func, &arr_info, ctx->synt->arrs) && 
            VRS_intext(node->first_child->token)
        );

        /*
        Generate offset movement in this array-like data type.
        Then multiply it by arr el_size.
        */
        _generate_expression(left->first_child, output, func, ctx);
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

    fprintf(output, " ; --------------- \n");
    return 1;
}

static int _generate_text_section(tree_t* node, FILE* output, gen_ctx_t* ctx) {
    if (!node) return 0;
    for (tree_t* child = node->first_child; child; child = child->next_sibling) {
        _generate_expression(child, output, NULL, ctx);
    }

    return 1;
}

int GEN_generate(gen_ctx_t* ctx, FILE* output) {
    tree_t* program_body = ctx->synt->r->first_child;
    tree_t* prefix_node  = program_body;
    tree_t* main_node    = prefix_node->next_sibling;

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
        for (tree_t* child = prefix_node->first_child; child; child = child->next_sibling) {
            if (!child->token) continue;
            switch (child->token->t_type) {
                case IMPORT_SELECT_TOKEN:
                    for (tree_t* func = child->first_child->first_child; func; func = func->next_sibling)
                        fprintf(output, "    extern __%s__\n", func->token->value);
                break;

                case FUNC_TOKEN:
                    fprintf(output, "    global __%s__\n", child->first_child->token->value);
                break;
                default: break;
            }
        }

        _generate_text_section(prefix_node, output, ctx);
    }

    /* Generate main section code */
    if (main_node) {
        fprintf(output, "\nglobal _start\n\n");
        fprintf(output, "    _start:\n");

        /* Save old stack and reserve new one */
        iprintf(output, "push %s\n", GET_RAW_REG(BASE_BITNESS, RBP));
        iprintf(output, "mov %s, %s\n", GET_RAW_REG(BASE_BITNESS, RBP), GET_RAW_REG(BASE_BITNESS, RSP));
        iprintf(output, "sub %s, %d\n", GET_RAW_REG(BASE_BITNESS, RSP), ALIGN(_get_variables_size(main_node->first_child, NULL, ctx)));
       
        _generate_text_section(main_node, output, ctx);
    }

    return 1;
}
