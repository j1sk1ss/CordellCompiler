#include <ir/x86_64_gnu_nasm/x86_64_irgen.h>

int IR_generate_if_block(ast_node_t* node, ir_gen_t* g, ir_ctx_t* ctx) {
    int current_label   = ctx->lid++;
    ast_node_t* cond    = node->child;
    ast_node_t* lbranch = cond->sibling;
    ast_node_t* rbranch = lbranch->sibling;

    g->elemegen(cond, g, ctx);
    IR_BLOCK2(ctx, iCMP, IR_SUBJ_REG(RAX, 8), IR_SUBJ_CONST(0));
    if (rbranch) IR_BLOCK1(ctx, JE, IR_SUBJ_LABEL("_f%d", current_label));
    else IR_BLOCK1(ctx, JE, IR_SUBJ_LABEL("_e%d", current_label));

    g->blockgen(lbranch->child, g, ctx);
    IR_BLOCK1(ctx, JMP, IR_SUBJ_LABEL("_e%d", current_label));

    if (rbranch) {
        IR_BLOCK1(ctx, MKLB, IR_SUBJ_LABEL("_f%d", current_label));
        g->blockgen(rbranch->child, g, ctx);
    }

    IR_BLOCK1(ctx, MKLB, IR_SUBJ_LABEL("_e%d", current_label));
    return 1;
}

int IR_generate_while_block(ast_node_t* node, ir_gen_t* g, ir_ctx_t* ctx) {
    int current_label   = ctx->lid++;
    ast_node_t* cond    = node->child;
    ast_node_t* lbranch = cond->sibling;
    ast_node_t* rbranch = lbranch->sibling;
    
    IR_BLOCK1(ctx, MKLB, IR_SUBJ_LABEL("_w%d", current_label));
    g->elemegen(cond, g, ctx);
    IR_BLOCK2(ctx, iCMP, IR_SUBJ_REG(RAX, 8), IR_SUBJ_CONST(0));
    IR_BLOCK1(ctx, JE, IR_SUBJ_LABEL("_e%d", current_label));
    
    g->blockgen(lbranch->child, g, ctx);
    IR_BLOCK1(ctx, JMP, IR_SUBJ_LABEL("_w%d", current_label));
    
    IR_BLOCK1(ctx, MKLB, IR_SUBJ_LABEL("_e%d", current_label));
    if (rbranch) g->blockgen(rbranch->child, g, ctx);
    return 1;
}

static int _generate_case_binary_jump(int* values, int left, int right, int label_id, int default_scope, ir_ctx_t* ctx) {
    if (left > right) {
        if (default_scope) IR_BLOCK1(ctx, JMP, IR_SUBJ_LABEL("_d%d", label_id));
        else IR_BLOCK1(ctx, JMP, IR_SUBJ_LABEL("_e%d", label_id));
        return 0;
    }

    int mid = (left + right) / 2;
    int val = values[mid];

    IR_BLOCK2(ctx, iCMP, IR_SUBJ_REG(RAX, 8), IR_SUBJ_CONST(val));
    IR_BLOCK1(ctx, JL, IR_SUBJ_LABEL("_cl%d%d", val, label_id));
    IR_BLOCK1(ctx, JG, IR_SUBJ_LABEL("_cr%d%d", val, label_id));
    IR_BLOCK1(ctx, JMP, IR_SUBJ_LABEL("_c%d%d", val, label_id));

    IR_BLOCK1(ctx, MKLB, IR_SUBJ_LABEL("_cl%d%d", val, label_id));
    _generate_case_binary_jump(values, left, mid - 1, label_id, default_scope, ctx);

    IR_BLOCK1(ctx, MKLB, IR_SUBJ_LABEL("_cr%d%d", val, label_id));
    _generate_case_binary_jump(values, mid + 1, right, label_id, default_scope, ctx);
    return 1;
}

static int _cmp(const void* a, const void* b) {
    return (*(int*)a - *(int*)b);
}

int IR_generate_switch_block(ast_node_t* node, ir_gen_t* g, ir_ctx_t* ctx) {
    int current_label = ctx->lid++;
    ast_node_t* cond  = node->child;
    ast_node_t* cases = cond->sibling;

    int cases_count = 0;
    int values[128] = { -1 };

    int def = 0;
    IR_BLOCK1(ctx, JMP, IR_SUBJ_LABEL("_ec%d", current_label));
    for (ast_node_t* curr_case = cases->child; curr_case; curr_case = curr_case->sibling) {
        if (curr_case->token->t_type == DEFAULT_TOKEN) {
            IR_BLOCK1(ctx, MKLB, IR_SUBJ_LABEL("_d%d", current_label));
            g->blockgen(curr_case->child, g, ctx);
            def = 1;
        }
        else {
            int case_value = str_atoi(curr_case->child->token->value);
            IR_BLOCK1(ctx, MKLB, IR_SUBJ_LABEL("_c%d%d", case_value, current_label));
            g->blockgen(curr_case->child->sibling, g, ctx);
            values[cases_count++] = case_value;
        }

        IR_BLOCK1(ctx, JMP, IR_SUBJ_LABEL("_e%d", current_label));
    }

    sort_qsort(values, cases_count, sizeof(int), _cmp);
    IR_BLOCK1(ctx, MKLB, IR_SUBJ_LABEL("_ec%d", current_label));
    g->blockgen(cond, g, ctx);
    _generate_case_binary_jump(values, 0, cases_count - 1, current_label, def, ctx);

    IR_BLOCK1(ctx, MKLB, IR_SUBJ_LABEL("_e%d", current_label));
    return 1;
}
