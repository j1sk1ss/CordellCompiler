#include <hir/hirgens/hirgens.h>

int HIR_generate_if_block(ast_node_t* node, hir_ctx_t* ctx, sym_table_t* smt) {
    ast_node_t* cond    = node->child;
    ast_node_t* lbranch = cond->sibling;
    ast_node_t* rbranch = lbranch->sibling;

    hir_subject_t* condtmp = HIR_generate_elem(cond, ctx, smt);
    hir_subject_t* flb = HIR_SUBJ_LABEL();
    hir_subject_t* elb = HIR_SUBJ_LABEL();
    if (rbranch) HIR_BLOCK2(ctx, HIR_IFOP, condtmp, flb);
    else HIR_BLOCK2(ctx, HIR_IFOP, condtmp, elb);

    if (lbranch) {
        HIR_BLOCK1(ctx, HIR_MKSCOPE, HIR_SUBJ_CONST(lbranch->sinfo.s_id));
        HIR_generate_block(lbranch->child, ctx, smt);
        HIR_BLOCK1(ctx, HIR_ENDSCOPE, HIR_SUBJ_CONST(lbranch->sinfo.s_id));
        if (rbranch) HIR_BLOCK1(ctx, HIR_JMP, elb);
    }

    if (rbranch) {
        HIR_BLOCK1(ctx, HIR_MKLB, flb);
        HIR_BLOCK1(ctx, HIR_MKSCOPE, HIR_SUBJ_CONST(rbranch->sinfo.s_id));
        HIR_generate_block(rbranch->child, ctx, smt);
        HIR_BLOCK1(ctx, HIR_ENDSCOPE, HIR_SUBJ_CONST(rbranch->sinfo.s_id));
    }

    HIR_BLOCK1(ctx, HIR_MKLB, elb);
    return 1;
}

int HIR_generate_while_block(ast_node_t* node, hir_ctx_t* ctx, sym_table_t* smt) {
    ast_node_t* cond    = node->child;
    ast_node_t* lbranch = cond->sibling;
    ast_node_t* rbranch = lbranch->sibling;
    
    hir_subject_t* entrylb = HIR_SUBJ_LABEL();
    hir_subject_t* elb = HIR_SUBJ_LABEL();

    HIR_BLOCK1(ctx, HIR_MKLB, entrylb);
    HIR_BLOCK2(ctx, HIR_IFOP, HIR_generate_elem(cond, ctx, smt), elb);
    if (lbranch) {
        HIR_BLOCK1(ctx, HIR_MKSCOPE, HIR_SUBJ_CONST(lbranch->sinfo.s_id));
        HIR_generate_block(lbranch->child, ctx, smt);
        HIR_BLOCK1(ctx, HIR_ENDSCOPE, HIR_SUBJ_CONST(lbranch->sinfo.s_id));
    }

    HIR_BLOCK1(ctx, HIR_JMP, entrylb);
    HIR_BLOCK1(ctx, HIR_MKLB, elb);
    if (rbranch) {
        HIR_BLOCK1(ctx, HIR_MKSCOPE, HIR_SUBJ_CONST(rbranch->sinfo.s_id));
        HIR_generate_block(rbranch->child, ctx, smt);
        HIR_BLOCK1(ctx, HIR_ENDSCOPE, HIR_SUBJ_CONST(rbranch->sinfo.s_id));
    }
    
    return 1;
}

typedef struct {
    long           v;
    hir_subject_t* l;
} binary_cases_t;

static int _generate_case_binary_jump(
    binary_cases_t* values, hir_subject_t* cond, 
    int left, int right,
    hir_subject_t* def, hir_subject_t* end, hir_ctx_t* ctx
) {
    if (left > right) {
        if (def) HIR_BLOCK1(ctx, HIR_JMP, def);
        else HIR_BLOCK1(ctx, HIR_JMP, end);
        return 0;
    }

    int mid = (left + right) / 2; 
    int val = values[mid].v;

    hir_subject_t* lower   = HIR_SUBJ_LABEL();
    hir_subject_t* greater = HIR_SUBJ_LABEL();

    HIR_BLOCK3(ctx, HIR_IFLWOP, cond, HIR_SUBJ_CONST(val), lower);
    HIR_BLOCK3(ctx, HIR_IFLGOP, cond, HIR_SUBJ_CONST(val), greater);
    HIR_BLOCK1(ctx, HIR_JMP, values[mid].l);

    HIR_BLOCK1(ctx, HIR_MKLB, lower);
    _generate_case_binary_jump(values, cond, left, mid - 1, def, end, ctx);

    HIR_BLOCK1(ctx, HIR_MKLB, greater);
    _generate_case_binary_jump(values, cond, mid + 1, right, def, end, ctx);
    return 1;
}

static int _cmp(const void* a, const void* b) {
    return (((binary_cases_t*)a)->v - ((binary_cases_t*)b)->v);
}

int HIR_generate_switch_block(ast_node_t* node, hir_ctx_t* ctx, sym_table_t* smt) {
    ast_node_t* cond  = node->child;
    ast_node_t* cases = cond->sibling;

    int cases_count = 0;
    binary_cases_t cases_info[128];

    hir_subject_t* def = NULL;
    hir_subject_t* cguards = HIR_SUBJ_LABEL();
    HIR_BLOCK1(ctx, HIR_JMP, cguards);

    hir_subject_t* end = HIR_SUBJ_LABEL();
    for (ast_node_t* curr_case = cases->child; curr_case; curr_case = curr_case->sibling) {
        if (curr_case->token->t_type == DEFAULT_TOKEN && !def) {
            hir_subject_t* clb = HIR_SUBJ_LABEL();
            HIR_BLOCK1(ctx, HIR_MKLB, clb);
            HIR_generate_block(curr_case->child, ctx, smt);
            def = clb;
        }
        else {
            hir_subject_t* clb = HIR_SUBJ_LABEL();
            HIR_BLOCK1(ctx, HIR_MKLB, clb);
            HIR_generate_block(curr_case->child->sibling, ctx, smt);
            cases_info[cases_count].v = str_atoi(curr_case->child->token->value);
            cases_info[cases_count].l = clb;
            cases_count++;
        }

        HIR_BLOCK1(ctx, HIR_JMP, end);
    }

    sort_qsort(cases_info, cases_count, sizeof(binary_cases_t), _cmp);
    HIR_BLOCK1(ctx, HIR_MKLB, cguards);
    
    hir_subject_t* condtmp = HIR_generate_elem(cond, ctx, smt);
    _generate_case_binary_jump(cases_info, condtmp, 0, cases_count - 1, def, end, ctx);
    HIR_BLOCK1(ctx, HIR_MKLB, end);
    return 1;
}
