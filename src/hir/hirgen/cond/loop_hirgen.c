#include <hir/hirgens/hirgens.h>

/* Pick the smallest stack integer type that can hold a positive loop count.
Params:
    - `val` - Counter initial value.

Returns the HIR stack variable type for the counter */
static inline hir_subject_type_t _get_valid_sizes(unsigned long val) {
    if (val <= CHAR_MAX)       return HIR_STKVARI8;
    else if (val <= UCHAR_MAX) return HIR_STKVARU8;
    else if (val <= SHRT_MAX)  return HIR_STKVARI16;
    else if (val <= USHRT_MAX) return HIR_STKVARU16;
    else if (val <= INT_MAX)   return HIR_STKVARI32;
    else if (val <= UINT_MAX)  return HIR_STKVARU32;
    else if (val <= LONG_MAX)  return HIR_STKVARI64;
    else                       return HIR_STKVARU64;
}

static inline void _read_counter_data(
    annotation_counter_t* counter_data, hir_ctx_t* ctx, sym_table_t* smt,
    hir_subject_t** index, hir_subject_t** step, int* index_owned_by_blocks
) {
    if (!counter_data || !counter_data->has_index) return;

    if (counter_data->idx_t == ANNOTATION_VALUE_PARAM) {
        hir_subject_type_t tt = _get_valid_sizes(counter_data->index.value);
        *index = HIR_SUBJ_STKVAR(VRTB_add_info(NULL, HIR_get_tmptkn_type(tt), NO_SYMBOL_ID, EMPTY_BASIC_FLAGS, &smt->v), tt, 0);
        HIR_BLOCK1(ctx, HIR_VARDECL, *index);
        HIR_BLOCK2(ctx, HIR_STORE, *index, HIR_SUBJ_CONST(counter_data->index.value));
        *index_owned_by_blocks = 1;
    }
    else {
        variable_info_t prev_vi;
        if (VRTB_get_info_id(counter_data->index.v_id, &prev_vi, &smt->v)) {
            *index = HIR_SUBJ_STKVAR(counter_data->index.v_id, HIR_get_stktype(&prev_vi), 0);
        }
    }

    if (!*index) return;
    if (!counter_data->has_step) return;

    if (counter_data->stp_t == ANNOTATION_VALUE_PARAM) *step = HIR_SUBJ_CONST(counter_data->step.value);
    else {
        variable_info_t prev_vi;
        if (VRTB_get_info_id(counter_data->step.v_id, &prev_vi, &smt->v)) {
            *step = HIR_SUBJ_STKVAR(counter_data->step.v_id, HIR_get_stktype(&prev_vi), 0);
        }
    }
}

/* Generates a counted (or not) loop.
Params:
    - `node` - Entry 'loop' node.
    - `ctx` - HIR context.
    - `counter_data` - Counter annotation data. Can be NULL which will create a regular loop.
    - `smt` - Symtable.

Returns 1 if succeeds */
static int _generate_counted_loop_block(ast_node_t* node, hir_ctx_t* ctx, annotation_counter_t* counter_data, sym_table_t* smt) {
    hir_subject_t *counter = NULL, *step = NULL;
    int counter_owned_by_blocks = 0;
    _read_counter_data(counter_data, ctx, smt, &counter, &step, &counter_owned_by_blocks);
    ast_node_t* lbranch = node->c;    
    if (lbranch) {
        hir_subject_t* entry_lb = HIR_SUBJ_LABEL();
        hir_subject_t* body_lb  = HIR_SUBJ_LABEL();
        hir_subject_t* end_lb   = HIR_SUBJ_LABEL();

        HIR_BLOCK1(ctx, HIR_MKLB, entry_lb);
        HIR_BLOCK1(ctx, HIR_JMP, body_lb);

        HIR_BLOCK1(ctx, HIR_MKLB, body_lb);
        HIR_BLOCK1(ctx, HIR_MKSCOPE, HIR_SUBJ_CONST(lbranch->sinfo.s_id));
        
        hir_subject_t* backup = ctx->carry.brk;
        ctx->carry.brk = end_lb;
        HIR_generate_block(lbranch->c, ctx, smt);
        ctx->carry.brk = backup;

        HIR_BLOCK1(ctx, HIR_ENDSCOPE, HIR_SUBJ_CONST(lbranch->sinfo.s_id));
        if (!counter) HIR_BLOCK1(ctx, HIR_JMP, entry_lb);
        else {
            if (!step) step = HIR_SUBJ_CONST(1);
            hir_subject_t* res = HIR_SUBJ_TMPVAR(
                counter->t, VRTB_add_info(NULL, HIR_get_tmptkn_type(counter->t), NO_SYMBOL_ID, EMPTY_BASIC_FLAGS, &smt->v)
            );
            HIR_BLOCK3(ctx, HIR_iSUB, res, HIR_copy_subject(counter), step);
            HIR_BLOCK2(ctx, HIR_STORE, HIR_copy_subject(counter), res);
            HIR_BLOCK3(ctx, HIR_IFOP2, HIR_copy_subject(counter), entry_lb, end_lb);
        }

        HIR_BLOCK1(ctx, HIR_MKLB, end_lb);
    }

    if (counter && !counter_owned_by_blocks) HIR_unload_subject(counter);
    return 1;
}

int HIR_generate_loop_block(ast_node_t* node, hir_ctx_t* ctx, sym_table_t* smt) {
    HIR_SET_CURRENT_POS(ctx, node);
    HAS_ANNOTATION(COUNTER_ANNOTATION, node, {
        return _generate_counted_loop_block(node, ctx, &annot->data.counter, smt);
    });

    return _generate_counted_loop_block(node, ctx, NULL, smt);
}
