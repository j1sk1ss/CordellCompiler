#include <generator.h>

int x86_64_generate_if(ast_node_t* node, FILE* output, gen_ctx_t* ctx) {
    int current_label   = ctx->label++;
    ast_node_t* cond    = node->child;
    ast_node_t* lbranch = cond->sibling;
    ast_node_t* rbranch = lbranch->sibling;

    x86_64_generate_block(cond, output, ctx); /* Condition gen */
    iprintf(output, "cmp %s, 0\n", GET_RAW_REG(BASE_BITNESS, RAX));
    if (rbranch) iprintf(output, "je __else_%d__\n", current_label);
    else iprintf(output, "je __end_if_%d__\n", current_label);

    x86_64_generate_block(lbranch, output, ctx); /* Main block gen */
    iprintf(output, "jmp __end_if_%d__\n", current_label);

    if (rbranch) { /* Else block gen */
        iprintf(output, "__else_%d__:\n", current_label);
        x86_64_generate_block(rbranch, output, ctx);
    }

    iprintf(output, "__end_if_%d__:\n", current_label);
    return 1;
}

int x86_64_generate_while(ast_node_t* node, FILE* output, gen_ctx_t* ctx) {
    int current_label   = ctx->label++;
    ast_node_t* cond    = node->child;
    ast_node_t* lbranch = cond->sibling;
    ast_node_t* rbranch = lbranch->sibling;

    iprintf(output, "__while_%d__:\n", current_label);
    x86_64_generate_block(cond, output, ctx);
    iprintf(output, "cmp rax, 0\n");
    iprintf(output, "je __end_while_%d__\n", current_label);

    x86_64_generate_block(lbranch, output, ctx); /* Main body gen */
    iprintf(output, "jmp __while_%d__\n", current_label);

    iprintf(output, "__end_while_%d__:\n", current_label);
    if (rbranch) {
        x86_64_generate_block(rbranch, output, ctx);
    }

    return 1;
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

static int _cmp(const void* a, const void* b) {
    return (*(int*)a - *(int*)b);
}

int x86_64_generate_switch(ast_node_t* node, FILE* output, gen_ctx_t* ctx) {
    int current_label = ctx->label++;
    ast_node_t* cond  = node->child;
    ast_node_t* cases = cond->sibling;

    int cases_count = 0;
    int values[128] = { -1 };

    int def = 0;
    iprintf(output, "jmp __end_cases_%d__\n", current_label);
    for (ast_node_t* curr_case = cases->child; curr_case; curr_case = curr_case->sibling) {
        if (curr_case->token->t_type == DEFAULT_TOKEN) {
            iprintf(output, "__default_%d__:\n", current_label);
            def = 1;
        }
        else {
            int case_value = str_atoi(curr_case->token->value);
            iprintf(output, "__case_%d_%d__:\n", case_value, current_label);
            values[cases_count++] = case_value;
        }

        x86_64_generate_block(curr_case->child, output, ctx);
        iprintf(output, "jmp __end_switch_%d__\n", current_label);
    }

    sort_qsort(values, cases_count, sizeof(int), _cmp);
    iprintf(output, "__end_cases_%d__:\n", current_label);
    x86_64_generate_block(cond, output, ctx);
    _generate_case_binary_jump(output, values, 0, cases_count - 1, current_label, def);

    iprintf(output, "__end_switch_%d__:\n", current_label);
    return 1;
}
