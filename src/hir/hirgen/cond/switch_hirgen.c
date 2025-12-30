#include <hir/hirgens/hirgens.h>

typedef struct {
    long           v;
    hir_subject_t* l;
} binary_cases_t;

static int _generate_case_binary_jump(
    binary_cases_t* values, hir_subject_t* cond, 
    int left, int right,
    hir_subject_t* def, hir_subject_t* end, hir_ctx_t* ctx, sym_table_t* smt
) {
    if (left > right) {
        if (def) HIR_BLOCK1(ctx, HIR_JMP, def);
        else HIR_BLOCK1(ctx, HIR_JMP, end);
        return 0;
    }

    int mid = (left + right) / 2; 
    int val = values[mid].v;

    hir_subject_t* lower         = HIR_SUBJ_LABEL();
    hir_subject_t* eq_or_greater = HIR_SUBJ_LABEL();
    hir_subject_t* greater       = HIR_SUBJ_LABEL();
    hir_subject_t* equals        = HIR_SUBJ_LABEL();

    hir_subject_t* is_lwr = HIR_SUBJ_TMPVAR(HIR_TMPVARI8, VRTB_add_info(NULL, TMP_I8_TYPE_TOKEN, 0, NULL, &smt->v));
    HIR_BLOCK3(ctx, HIR_iLWR, is_lwr, cond, HIR_generate_conv(ctx, cond->t, HIR_SUBJ_CONST(val), smt));
    HIR_BLOCK3(ctx, HIR_IFOP2, is_lwr, lower, eq_or_greater);
    HIR_BLOCK1(ctx, HIR_MKLB, lower);
    _generate_case_binary_jump(values, cond, left, mid - 1, def, end, ctx, smt);

    HIR_BLOCK1(ctx, HIR_MKLB, eq_or_greater);
    hir_subject_t* is_grt = HIR_SUBJ_TMPVAR(HIR_TMPVARI8, VRTB_add_info(NULL, TMP_I8_TYPE_TOKEN, 0, NULL, &smt->v));
    HIR_BLOCK3(ctx, HIR_iLRG, is_grt, cond, HIR_generate_conv(ctx, cond->t, HIR_SUBJ_CONST(val), smt));
    HIR_BLOCK3(ctx, HIR_IFOP2, is_grt, greater, equals);
    HIR_BLOCK1(ctx, HIR_MKLB, greater);
    _generate_case_binary_jump(values, cond, mid + 1, right, def, end, ctx, smt);

    HIR_BLOCK1(ctx, HIR_MKLB, equals);
    HIR_BLOCK1(ctx, HIR_JMP, values[mid].l);
    return 1;
}

static int _cmp(const void* a, const void* b) {
    return (((binary_cases_t*)a)->v - ((binary_cases_t*)b)->v);
}

int HIR_generate_switch_block(ast_node_t* node, hir_ctx_t* ctx, sym_table_t* smt) {
    ast_node_t* cond  = node->c;
    ast_node_t* cases = cond->siblings.n;

    int cases_count = 0;
    binary_cases_t cases_info[128];

    hir_subject_t* def = NULL;
    hir_subject_t* cguards = HIR_SUBJ_LABEL();
    HIR_BLOCK1(ctx, HIR_JMP, cguards);

    hir_subject_t* end = HIR_SUBJ_LABEL();
    for (ast_node_t* curr_case = cases->c; curr_case; curr_case = curr_case->siblings.n) {
        hir_subject_t* clb = HIR_SUBJ_LABEL();
        HIR_BLOCK1(ctx, HIR_MKLB, clb);
        
        if (curr_case->t->t_type == DEFAULT_TOKEN && !def) {
            HIR_generate_block(curr_case->c, ctx, smt);
            def = clb;
        }
        else {
            HIR_generate_block(curr_case->c->siblings.n, ctx, smt);
            cases_info[cases_count].v = curr_case->c->t->body->to_llong(curr_case->c->t->body);
            cases_info[cases_count].l = clb;
            cases_count++;
        }

        HIR_BLOCK1(ctx, HIR_JMP, end);
    }

    sort_qsort(cases_info, cases_count, sizeof(binary_cases_t), _cmp);
    HIR_BLOCK1(ctx, HIR_MKLB, cguards);
    
    hir_subject_t* condtmp = HIR_generate_elem(cond, ctx, smt);
    _generate_case_binary_jump(cases_info, condtmp, 0, cases_count - 1, def, end, ctx, smt);
    HIR_BLOCK1(ctx, HIR_MKLB, end);
    return 1;
}
