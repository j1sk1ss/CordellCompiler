#include <hir/hirgens/hirgens.h>

static inline void _generate_logic_check(
    hir_subject_t* l, hir_subject_t* r,
    hir_subject_t* tr, hir_operation_t op, hir_subject_t* fl, 
    hir_ctx_t* ctx, sym_table_t* smt
) {
    hir_subject_t* is_true = HIR_SUBJ_TMPVAR(HIR_TMPVARI8, VRTB_add_info(NULL, TMP_I8_TYPE_TOKEN, NO_SYMBOL_ID, EMPTY_BASIC_FLAGS, &smt->v));
    HIR_BLOCK3(ctx, op, is_true, HIR_generate_implconv(ctx, 0, HIR_TMPVARI64, l, smt), r);
    HIR_BLOCK3(ctx, HIR_IFOP2, is_true, tr, fl);
}

typedef struct {
    long           v;
    hir_subject_t* l;
} case_info_t;

static int _generate_case_binary_jump(
    case_info_t* values, hir_subject_t* cond, 
    int left, int right,
    hir_subject_t* def, hir_ctx_t* ctx, sym_table_t* smt
) {
    if (left > right) {
        HIR_BLOCK1(ctx, HIR_JMP, def);
        return 0;
    }

    int mid = (left + right) / 2; 
    int val = values[mid].v;

    hir_subject_t* lower         = HIR_SUBJ_LABEL();
    hir_subject_t* eq_or_greater = HIR_SUBJ_LABEL();
    hir_subject_t* greater       = HIR_SUBJ_LABEL();
    hir_subject_t* equals        = HIR_SUBJ_LABEL();
    hir_subject_t* value         = HIR_generate_implconv(ctx, 0, HIR_TMPVARI64, HIR_SUBJ_CONST(val), smt);

    _generate_logic_check(cond, value, lower, HIR_iLWR, eq_or_greater, ctx, smt);
    HIR_BLOCK1(ctx, HIR_MKLB, lower);
    _generate_case_binary_jump(values, cond, left, mid - 1, def, ctx, smt);

    HIR_BLOCK1(ctx, HIR_MKLB, eq_or_greater);

    _generate_logic_check(cond, value, greater, HIR_iLRG, equals, ctx, smt);
    HIR_BLOCK1(ctx, HIR_MKLB, greater);
    _generate_case_binary_jump(values, cond, mid + 1, right, def, ctx, smt);

    HIR_BLOCK1(ctx, HIR_MKLB, equals);
    HIR_BLOCK1(ctx, HIR_JMP, values[mid].l);
    return 1;
}

static int _cmp(const void* a, const void* b) {
    return (((case_info_t*)a)->v - ((case_info_t*)b)->v);
}

static int _generate_sequent_jump(
    case_info_t* values, int values_count, hir_subject_t* cond, hir_subject_t* def, hir_ctx_t* ctx, sym_table_t* smt
) {
    for (int i = 0; i < values_count; i++) {
        hir_subject_t* nl     = HIR_SUBJ_LABEL();
        hir_subject_t* equals = HIR_SUBJ_TMPVAR(HIR_TMPVARI8, VRTB_add_info(NULL, TMP_I8_TYPE_TOKEN, NO_SYMBOL_ID, EMPTY_BASIC_FLAGS, &smt->v));
        hir_subject_t* value  = HIR_generate_implconv(ctx, 0, HIR_TMPVARI64, HIR_SUBJ_CONST(values[i].v), smt);
        HIR_BLOCK3(ctx, HIR_iCMP, equals, HIR_generate_implconv(ctx, 0, HIR_TMPVARI64, cond, smt), value);
        HIR_BLOCK3(ctx, HIR_IFOP2, equals, values[i].l, nl);
        HIR_BLOCK1(ctx, HIR_MKLB, nl);
    }

    if (def) HIR_BLOCK1(ctx, HIR_JMP, def);
    return 1;
}

/* Main idea here to create a new array that contains every label from the switch statement.
   The final array will look like this:
    _jump_array: case1_lb, case2_lb, def_lb, case3_lb
*/
static int _generate_table_jump(case_info_t* values, int values_count, hir_subject_t* cond, hir_subject_t* def, hir_ctx_t* ctx, sym_table_t* smt) {
    symbol_id_t j_table = VRTB_add_info(NULL, ARRAY_TYPE_TOKEN, NO_SYMBOL_ID,  (basic_object_info_t) { .glob = 1, .ro = 1 },  &smt->v);
    ARTB_add_info(j_table, values_count, 0, U64_TYPE_TOKEN, (basic_object_info_t) { .ptr = 1 }, &smt->a);

    string_t* ro_section = create_string(CONF_get_ro_section());
    SCTB_add_to_section(ro_section, FIELD_NO_CHANGE, j_table, SECTION_ELEMENT_VARIABLE, &smt->c);
    destroy_string(ro_section);

    for (int i = 0; i < values_count; i++) {
        ARTB_add_elems(j_table, (array_elem_info_t) { .t = ARRAY_ELEM_LABEL_TYPE, .s.value = values[i].l->id }, &smt->a);
        if (i + 1 < values_count) {
            for (int j = values[i].v; j < values[i + 1].v - 1; j++) {
                ARTB_add_elems(j_table, (array_elem_info_t) { .t = ARRAY_ELEM_LABEL_TYPE, .s.value = def->id }, &smt->a);
            }
        }
    }

    hir_subject_t* check_down = HIR_SUBJ_LABEL();
    _generate_logic_check(cond, HIR_SUBJ_CONST(values[0].v), def, HIR_iLWR, check_down, ctx, smt);

    HIR_BLOCK1(ctx, HIR_MKLB, check_down);

    hir_subject_t* proceed_jumps = HIR_SUBJ_LABEL();
    _generate_logic_check(cond, HIR_SUBJ_CONST(values[values_count - 1].v), def, HIR_iLRG, proceed_jumps, ctx, smt);

    HIR_BLOCK1(ctx, HIR_MKLB, proceed_jumps);

    hir_subject_t* j_table_sbj = HIR_SUBJ_STKVAR(j_table, HIR_GLBVARARR, 0);
    hir_subject_t* head = HIR_reference_subject(j_table_sbj, smt, 1);
    HIR_BLOCK2(ctx, HIR_REF, head, j_table_sbj);

    hir_subject_t *index  = HIR_SUBJ_TMPVAR(HIR_TMPVARI64, VRTB_ADD_TMP_NF(HIR_TMPVARI64, smt)),
                  *offset = HIR_SUBJ_TMPVAR(HIR_TMPVARI64, VRTB_ADD_TMP_NF(HIR_TMPVARI64, smt));
    HIR_BLOCK3(ctx, HIR_iSUB, index, HIR_generate_implconv(ctx, 0, HIR_TMPVARI64, cond, smt), HIR_SUBJ_CONST(values[0].v));
    HIR_BLOCK3(ctx, HIR_iMUL, offset, index, HIR_SUBJ_CONST(CONF_get_full_bytness()));

    hir_subject_t* target_head = HIR_SUBJ_CPVAR(head, smt);
    target_head->ptr = head->ptr;
    HIR_BLOCK3(ctx, HIR_iADD, target_head, head, HIR_generate_implconv(ctx, head->ptr, head->t, offset, smt));

    hir_subject_t* target = HIR_gdref_subject(target_head, smt, ctx, FIELD_NO_CHANGE);
    HIR_BLOCK2(ctx, HIR_JMP, target, HIR_copy_subject(j_table_sbj));
    return 1;
}

static inline int _select_the_best_switch_type(case_info_t* values, int values_count) {
    int summary_gap = 0;
    for (int i = 0; i < values_count - 1; i++) {
        summary_gap += values[i + 1].v - values[i].v;
    }

    if (values_count < 5)               return SELECT_STRAIGHT;
    if (summary_gap / values_count < 3) return SELECT_JTABLE;
    if (values_count < 10)              return SELECT_STRAIGHT;
    return SELECT_BINARY; 
}

int HIR_generate_switch_block(ast_node_t* node, hir_ctx_t* ctx, sym_table_t* smt) {
    HIR_SET_CURRENT_POS(ctx, node);
    int no_fall = 0;
    HAS_ANNOTATION(NOFALL_ANNOTATION, node, { no_fall  = 1; });

    ast_node_t* cond  = node->c;
    ast_node_t* cases = cond->siblings.n;

    int cases_count = 0;
    case_info_t cases_info[128];

    hir_subject_t* def = NULL;
    hir_subject_t* cguards = HIR_SUBJ_LABEL();
    HIR_BLOCK1(ctx, HIR_JMP, cguards);

    hir_subject_t* end_lb = HIR_SUBJ_LABEL();
    for (ast_node_t* curr_case = cases->c; curr_case; curr_case = curr_case->siblings.n) {        
        hir_subject_t* backup = ctx->carry.brk;
        ctx->carry.brk = end_lb;

        int prev_cold = ctx->is_cold, fall = 0;
        HAS_ANNOTATION(COLD_ANNOTATION, curr_case, { ctx->is_cold = 1; });
        HAS_ANNOTATION(FALL_ANNOTATION, curr_case, { fall         = 1; });

        if ((!no_fall || fall) && ctx->is_cold) {
            HIRGEN_ERROR(ctx, "No fall switch with a cold section can create UB code!");
        }

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

        ctx->carry.brk = backup;
        if (no_fall && !fall) {
            HIR_BLOCK1(ctx, HIR_JMP, end_lb);
        }

        ctx->is_cold = prev_cold;
    }

    if (!def) def = end_lb;
    HIR_BLOCK1(ctx, HIR_JMP,  end_lb);
    HIR_BLOCK1(ctx, HIR_MKLB, cguards);
    hir_subject_t* condtmp = HIR_generate_elem(cond, ctx, smt);

    int switch_type = SELECT_AUTO;
    HAS_ANNOTATION(SWITCH_ANNOTATION, node, {
        annotations_summary_t as;
        ANNOT_read_annotation(annot, &as);
        switch_type = as.switch_type;
    })

    if (switch_type == SELECT_AUTO) {
        switch_type = _select_the_best_switch_type(cases_info, cases_count);
    }

    sort_qsort(cases_info, cases_count, sizeof(case_info_t), _cmp);
    switch (switch_type) {
        case SELECT_STRAIGHT: _generate_sequent_jump(cases_info, cases_count, condtmp, def, ctx, smt);            break;
        case SELECT_JTABLE:   _generate_table_jump(cases_info, cases_count, condtmp, def, ctx, smt);              break;
        default:
        case SELECT_BINARY:   _generate_case_binary_jump(cases_info, condtmp, 0, cases_count - 1, def, ctx, smt); break;
    }

    HIR_BLOCK1(ctx, HIR_MKLB, end_lb);
    return 1;
}
