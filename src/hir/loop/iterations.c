#include <hir/loop.h>

typedef struct {
    loop_node_t* loop;
    sym_table_t* smt;
    map_t        definitions;
    map_t        definition_blocks;
    set_t*       iteration_dependency_ids;
    int          failed;
} loop_iteration_ctx_t;

static inline int _add_iteration_dependency_id(loop_iteration_ctx_t* ctx, hir_subject_t* s) {
    if (
        !s || !HIR_is_vartype(s->t) ||
        set_has(ctx->iteration_dependency_ids, (void*)s->storage.var.v_id) ||
        set_add(ctx->iteration_dependency_ids, (void*)s->storage.var.v_id)
    ) return 1;
    ctx->failed = 1;
    return 0;
}

static hir_block_t* _find_definition(loop_iteration_ctx_t* ctx, hir_subject_t* s, cfg_block_t** owner) {
    hir_block_t* hh = NULL;
    if (
        !s || !HIR_is_vartype(s->t) || !_add_iteration_dependency_id(ctx, s) ||
        !map_get(&ctx->definitions, s->storage.var.v_id, (void**)&hh)
    ) return NULL;

    if (owner) map_get(&ctx->definition_blocks, s->storage.var.v_id, (void**)owner);
    return hh;
}

static int _get_constant_value(hir_subject_t* s, big_int_t* value, sym_table_t* smt) {
    value_interval_t interval = HIR_INT_get_max_subject_value_interval(s);
    if (!interval.found) return 0;
    if (HIR_is_vartype(s->t)) {
        variable_info_t vi;
        if (
            !VRTB_get_info_id(s->storage.var.v_id, &vi, &smt->v) || 
            vi.vdi.defined != DEFINED_VARIABLE
        ) return 0;
        *value = HIR_is_sign(s->t) ? (big_int_t)vi.vdi.definition : (big_int_t)(unsigned long)vi.vdi.definition;
    }
    else if (HIR_is_defined_type(s->t) == 2) *value = HIR_is_sign(s->t) ? (big_int_t)s->storage.cnst.value : (big_int_t)(unsigned long)s->storage.cnst.value;
    else                                     *value = (big_int_t)s->storage.num.value->to_ullong(s->storage.num.value, 10);
    return *value >= interval.low && *value <= interval.high;
}

static hir_subject_t* _find_source_subj(loop_iteration_ctx_t* ctx, hir_subject_t* s) {
    int attempts = 128;
    while (attempts-- > 0) {
        hir_block_t* hh = _find_definition(ctx, s, NULL);
        if (!hh || hh->op != HIR_STORE)                 return s;
        if (!HIR_INT_same_value_intervals(s, hh->sarg)) return NULL;
        s = hh->sarg;
    }

    return NULL;
}

static inline int _is_same_variable(hir_subject_t* a, hir_subject_t* b) {
    return a && b && HIR_is_vartype(a->t) && HIR_is_vartype(b->t) && a->storage.var.v_id == b->storage.var.v_id;
}

static hir_operation_t _invert_comparison(hir_operation_t op) {
    switch (op) {
        case HIR_iLWR: return HIR_iLGE;
        case HIR_iLRE: return HIR_iLRG;
        case HIR_iLRG: return HIR_iLRE;
        case HIR_iLGE: return HIR_iLWR;
        case HIR_iCMP: return HIR_iNMP;
        case HIR_iNMP: return HIR_iCMP;
        default:       return HIR_NOP;
    }
}

static hir_operation_t _swap_comparison_operands(hir_operation_t op) {
    switch (op) {
        case HIR_iLWR: return HIR_iLRG;
        case HIR_iLRE: return HIR_iLGE;
        case HIR_iLRG: return HIR_iLWR;
        case HIR_iLGE: return HIR_iLRE;
        default:       return op;
    }
}

static int _evaluate_comparison(hir_operation_t op, big_int_t a, big_int_t b) {
    switch (op) {
        case HIR_iLWR: return a < b;
        case HIR_iLRE: return a <= b;
        case HIR_iLRG: return a > b;
        case HIR_iLGE: return a >= b;
        case HIR_iCMP: return a == b;
        case HIR_iNMP: return a != b;
        default:       return 0;
    }
}

static hir_subject_t* _find_defined_subject(loop_iteration_ctx_t* ctx, symbol_id_t id) {
    hir_block_t* hh = NULL;
    if (!map_get(&ctx->definitions, id, (void**)&hh)) return NULL;
    return hh->op == HIR_PHI ? hh->sarg : hh->farg;
}

static int _gather_loop_iterator_info(
    loop_iteration_ctx_t* ctx, loop_node_t* loop, hir_block_t* main_phi, loop_iterator_info_t* out
) {
    if (
        !loop->latch || !loop->preheader ||
        set_size(&main_phi->targ->storage.set.h) != 2
    ) return 0;

    hir_subject_t* counter = main_phi->sarg;
    value_interval_t interval = HIR_INT_get_max_subject_value_interval(counter);
    if (!interval.found) return 0;

    out->next_id = out->initial_id = NO_SYMBOL_ID;
    set_foreach (int_tuple_t* incoming, &main_phi->targ->storage.set.h) {
        if (
            incoming->x == loop->preheader->id &&
            out->initial_id == NO_SYMBOL_ID
        ) out->initial_id = incoming->y;
        else if (
            incoming->x == loop->latch->id &&
            out->next_id == NO_SYMBOL_ID
        ) out->next_id = incoming->y;
        else return 0;
    }

    if (!_add_iteration_dependency_id(ctx, counter)) return 0;

    out->iterator = counter;
    out->phi      = main_phi;
    out->initial  = _find_defined_subject(ctx, out->initial_id);
    out->next     = _find_defined_subject(ctx, out->next_id);

    hir_subject_t value = *counter;
    value.storage.var.v_id = out->initial_id;
    if (!_add_iteration_dependency_id(ctx, &value)) return 0;
    out->initial_known = _get_constant_value(&value, &out->initial_value, ctx->smt);

    hir_subject_t* update_value = _find_source_subj(ctx, out->next);
    hir_block_t* update = _find_definition(ctx, update_value, &out->update_block);
    if (
        !update || !set_has(&ctx->loop->blocks, out->update_block) ||
        !HIR_INT_same_value_intervals(counter, update->farg) ||
        (update->op != HIR_iADD && update->op != HIR_iSUB) // LATER: can be upgraded to / and * with some additional code
    ) return 0;

    hir_subject_t *base = _find_source_subj(ctx, update->sarg), *amount = update->targ;
    if (!_is_same_variable(base, counter) && update->op == HIR_iADD) {
        base   = _find_source_subj(ctx, update->targ);
        amount = update->sarg;
    }

    if (
        !_is_same_variable(base, counter) || 
        !HIR_INT_suitable(amount, counter) ||
        !_add_iteration_dependency_id(ctx, amount)
    ) return 0;

    out->update     = update;
    out->step       = amount;
    out->step_known = _get_constant_value(amount, &out->step_value, ctx->smt);
    if (
        out->step_known && 
        update->op == HIR_iSUB
    ) out->step_value = -out->step_value;
    return 1;
}

static int _gather_iteration_condition(
    loop_iteration_ctx_t* ctx, loop_iterator_info_t* iterator, loop_iteration_condition_t* out
) {
    cfg_block_t* bb         = out->block;
    hir_subject_t* counter  = iterator->iterator;
    hir_subject_t* tested   = _find_source_subj(ctx, out->posttested ? iterator->next : counter);
    hir_subject_t* test     = _find_source_subj(ctx, out->branch->farg);
    hir_subject_t* bound    = NULL;
    hir_block_t* comparison = NULL;
    hir_operation_t op      = HIR_iNMP;

    if (!_is_same_variable(test, tested)) {
        cfg_block_t* comparison_block = NULL;
        comparison = _find_definition(ctx, test, &comparison_block);
        if (
            !comparison || comparison_block != bb ||
            _invert_comparison(comparison->op) == HIR_NOP
        ) return 0;

        op = comparison->op;
        hir_subject_t* lhs = _find_source_subj(ctx, comparison->sarg);
        bound = comparison->targ;
        if (!_is_same_variable(lhs, tested)) {
            lhs   = _find_source_subj(ctx, comparison->targ);
            bound = comparison->sarg;
            op    = _swap_comparison_operands(op);
        }

        if (
            !_is_same_variable(lhs, tested) || 
            !HIR_INT_suitable(bound, counter)
        ) return 0;
    }

    if (!_add_iteration_dependency_id(ctx, bound)) return 0;
    out->iterator    = iterator;
    out->comparison  = comparison;
    out->bound       = bound;
    out->op          = set_has(&ctx->loop->blocks, bb->l) ? op : _invert_comparison(op);
    out->bound_value = 0;
    out->bound_known = !bound || _get_constant_value(bound, &out->bound_value, ctx->smt);
    return 1;
}

static loop_iteration_status_t _calculate_iteration_count(
    loop_iterator_info_t* iterator, loop_iteration_condition_t* condition, cfg_block_t* header, big_int_t* count
) {
    value_interval_t interval = HIR_INT_get_max_subject_value_interval(iterator->iterator);
    if (
        !iterator->initial_known || !condition->bound_known ||
        !interval.found ||
        (!condition->posttested && iterator->update_block == header)
    ) return LOOP_ITERATION_UNKNOWN;

    big_int_t initial  = iterator->initial_value;
    big_int_t bound    = condition->bound_value;
    big_int_t step     = iterator->step_value;
    hir_operation_t op = condition->op;

    if (bound < interval.low || bound > interval.high) return LOOP_ITERATION_UNKNOWN;
    if (condition->posttested) {
        if (!iterator->step_known) return LOOP_ITERATION_UNKNOWN;
        initial += step;
    }

    if (initial < interval.low || initial > interval.high) return LOOP_ITERATION_UNKNOWN;
    if (!_evaluate_comparison(op, initial, bound)) {
        *count = condition->posttested ? 1 : 0;
        return LOOP_ITERATION_FINITE;
    }

    if (!iterator->step_known) return LOOP_ITERATION_UNKNOWN;
    if (!step) {
        *count = LOOP_ITERATION_COUNT_INFINITE;
        return LOOP_ITERATION_INFINITE;
    }

    big_int_t iterations;
    if ((op == HIR_iLWR || op == HIR_iLRE) && step > 0) {
        big_int_t distance = bound - initial + (op == HIR_iLRE);
        iterations = (distance + step - 1) / step;
    }
    else if ((op == HIR_iLRG || op == HIR_iLGE) && step < 0) {
        big_int_t distance = initial - bound + (op == HIR_iLGE);
        iterations = (distance - step - 1) / -step;
    }
    else if (op == HIR_iNMP) {
        big_int_t distance = bound - initial;
        if (distance % step || distance / step <= 0) return LOOP_ITERATION_UNKNOWN;
        iterations = distance / step;
    }
    else if (op == HIR_iCMP) iterations = 1;
    else return LOOP_ITERATION_UNKNOWN;

    big_int_t last = initial + iterations * step;
    if (last < interval.low || last > interval.high) return LOOP_ITERATION_UNKNOWN;

    *count = iterations + condition->posttested;
    return LOOP_ITERATION_FINITE;
}

static int _gather_loop_conditions(loop_node_t* loop, loop_iteration_info_t* out) {
    set_t conditions;
    if (!set_init(&conditions, SET_NO_CMP)) return 0;

    int success = 0;
    if (!HIR_LOOP_get_cond_blocks(loop, &conditions)) goto _done;

    set_foreach (cfg_block_t* bb, &conditions) {
        loop_iteration_condition_t* condition = mm_malloc(sizeof(loop_iteration_condition_t));
        if (!condition) goto _done;

        str_memset(condition, 0, sizeof(loop_iteration_condition_t));
        condition->block      = bb;
        condition->branch     = bb->hmap.exit;
        condition->op         = HIR_NOP;
        condition->posttested = bb == loop->latch && bb != loop->header;

        if (!list_push_back(&out->conditions, condition)) {
            mm_free(condition);
            goto _done;
        }
    }

    success = 1;

_done: {}
    set_free(&conditions);
    return success;
}

static int _can_count_loop_iterations(loop_node_t* loop, list_t* conditions) {
    if (
        !loop->latch || list_size(&loop->children) ||
        list_size(conditions) > 1
    ) return 0;

    loop_iteration_condition_t* condition = list_get_head(conditions);
    if (
        condition &&
        condition->block != loop->header && condition->block != loop->latch
    ) return 0;

    set_foreach (cfg_block_t* exit, &loop->exits) {
        if (set_has(&loop->blocks, exit)) continue;
        set_foreach (cfg_block_t* pred, &exit->pred) {
            if (
                set_has(&loop->blocks, pred) &&
                (!condition || pred != condition->block)
            ) return 0;
        }
    }

    return 1;
}

static int _gather_function_definitions(loop_iteration_ctx_t* ctx) {
    foreach (cfg_block_t* bb, &ctx->loop->header->pfunc->blocks) {
        iterate_hir_instructions (bb) {
            hir_subject_t* result = NULL;
            if (hh->op == HIR_PHI)           result = hh->sarg;
            else if (HIR_is_writeop(hh->op)) result = hh->farg;

            if (!result || !HIR_is_vartype(result->t)) continue;

            symbol_id_t id = result->storage.var.v_id;
            if (map_get(&ctx->definitions, id, NULL)) return -1;
            if (
                !map_put(&ctx->definitions, id, hh) ||
                !map_put(&ctx->definition_blocks, id, bb)
            ) return 0;
        }
    }

    return 1;
}

static int _analyze_loop_iterators(loop_iteration_ctx_t* ctx, loop_iteration_info_t* out, int can_count) {
    iterate_hir_instructions (ctx->loop->header) {
        if (hh->op != HIR_PHI) continue;

        set_t dependencies;
        if (!set_init(&dependencies, SET_NO_CMP)) return 0;

        ctx->iteration_dependency_ids = &dependencies;
        loop_iterator_info_t iterator = { 0 };
        if (!_gather_loop_iterator_info(ctx, ctx->loop, hh, &iterator)) {
            set_free(&dependencies);
            if (ctx->failed) return 0;
            continue;
        }

        loop_iterator_info_t* record = mm_malloc(sizeof(loop_iterator_info_t));
        if (!record || !list_push_back(&out->iterators, record)) {
            mm_free(record);
            set_free(&dependencies);
            return 0;
        }
        *record = iterator;

        foreach (loop_iteration_condition_t* condition, &out->conditions) {
            if (!_gather_iteration_condition(ctx, record, condition) || !can_count) continue;

            big_int_t count = LOOP_ITERATION_COUNT_UNKNOWN;
            loop_iteration_status_t status = _calculate_iteration_count(record, condition, ctx->loop->header, &count);
            if (status == LOOP_ITERATION_UNKNOWN) continue;

            out->status = status;
            out->count  = count;
            set_free(&out->iteration_dependency_ids);
            if (!set_copy(&out->iteration_dependency_ids, &dependencies)) ctx->failed = 1;
        }

        set_free(&dependencies);
        if (ctx->failed) return 0;
    }

    return 1;
}

static int _analyze_constant_condition(loop_iteration_ctx_t* ctx, loop_iteration_info_t* out) {
    loop_iteration_condition_t* condition = list_get_head(&out->conditions);
    if (!condition) {
        out->status = LOOP_ITERATION_INFINITE;
        out->count  = LOOP_ITERATION_COUNT_INFINITE;
        return 1;
    }

    big_int_t value;
    hir_subject_t* test = condition->branch->farg;
    ctx->iteration_dependency_ids = &out->iteration_dependency_ids;
    if (!_get_constant_value(test, &value, ctx->smt)) return 1;
    if (!_add_iteration_dependency_id(ctx, test))    return 0;

    int continuing = set_has(&ctx->loop->blocks, condition->block->l) ? value != 0 : value == 0;
    out->status = continuing ? LOOP_ITERATION_INFINITE : LOOP_ITERATION_FINITE;
    out->count  = continuing ? LOOP_ITERATION_COUNT_INFINITE : condition->posttested;
    return 1;
}

int HIR_LOOP_analyze_iterations(loop_node_t* loop, loop_iteration_info_t* out, sym_table_t* smt) {
    if (!out) return 0;
    *out = (loop_iteration_info_t) {
        .status = LOOP_ITERATION_UNKNOWN,
        .count  = LOOP_ITERATION_COUNT_UNKNOWN
    };

    list_init(&out->iterators);
    list_init(&out->conditions);

    if (
        !set_init(&out->iteration_dependency_ids, SET_NO_CMP) ||
        !loop || !loop->header || !loop->header->pfunc || !smt ||
        !_gather_loop_conditions(loop, out)
    ) return 0;

    loop_iteration_ctx_t ctx = { .loop = loop, .smt = smt };
    int success = 0;
    if (
        !map_init(&ctx.definitions, MAP_NO_CMP) ||
        !map_init(&ctx.definition_blocks, MAP_NO_CMP)
    ) goto _done;

    int definitions = _gather_function_definitions(&ctx);
    if (!definitions) goto _done;
    if (definitions < 0) {
        success = 1;
        goto _done;
    }

    int can_count = _can_count_loop_iterations(loop, &out->conditions);

    if (!_analyze_loop_iterators(&ctx, out, can_count)) goto _done;
    if (
        can_count && out->status == LOOP_ITERATION_UNKNOWN &&
        !_analyze_constant_condition(&ctx, out)
    ) goto _done;

    success = 1;

_done: {}
    map_free(&ctx.definitions);
    map_free(&ctx.definition_blocks);
    if (!success) {
        out->status = LOOP_ITERATION_UNKNOWN;
        out->count  = LOOP_ITERATION_COUNT_UNKNOWN;
    }

    return success;
}

void HIR_LOOP_free_iteration_info(loop_iteration_info_t* info) {
    if (!info) return;
    list_free_force(&info->iterators);
    list_free_force(&info->conditions);
    set_free(&info->iteration_dependency_ids);
}
