#include <sem/hir/hir_visitors.h>

static inline char* _format_location(trace_location_t* loc) {
    static char buff[256] = { 0 };
    if (loc->file) snprintf(buff, sizeof(buff), "[%s:%li:%li]", loc->file->body, loc->line, loc->column);
    else snprintf(buff, sizeof(buff), "[%li:%li]", loc->line, loc->column);
    return buff;
}

static const char* _resolve_variable_name(symbol_id_t id, sym_table_t* smt) {
    variable_info_t vi;
    do {
        if (VRTB_get_info_id(id, &vi, &smt->v)) {
            id = vi.p_id;
        }
    } while (id != NO_SYMBOL_ID);
    return vi.name->body;
}

static int _print_trace(trace_t* t) {
    list_iter_t it;
    list_iter_tinit(&t->messages, &it);
    trace_message_t* msg = list_iter_prev(&it);
    if (msg) SEMANTIC_WARNING(" %s %s ", _format_location(&msg->location), msg->message->body);
    while ((msg = list_iter_prev(&it))) {
        SEMANTIC_WARNING(" %s     %s ", _format_location(&msg->location), msg->message->body);
    }

    return 1;
}

int HIRWLKR_visit_setpos_instruction(HIR_VISITOR_ARGS) {
    HIR_VISITOR_ARGS_USE;
    str_memcpy(&ctx->prev_location, &ctx->curr_location, sizeof(ctx->curr_location));
    ctx->curr_location.column = b->farg->storage.pos.column;
    ctx->curr_location.line   = b->farg->storage.pos.line;
    ctx->curr_location.file   = b->farg->storage.pos.file;
    return 1;
}

int HIRWLKR_visit_phi_instruction(HIR_VISITOR_ARGS) {
    HIR_VISITOR_ARGS_USE;
    list_t* vars = (list_t*)mm_malloc(sizeof(list_t));
    if (!vars) return 0;
    list_init(vars);
    
    set_foreach (int_tuple_t* t, &b->targ->storage.set.h) {
        list_add(vars, (void*)t->y);
    }
    
    return map_put(&ctx->definitions, (long)b->sarg->storage.var.v_id, (void*)vars);
}

typedef struct {
    long const_value;
    char defined_value;
} defined_variable_t;

/*
1 - defined raw number or a constant
2 - defined variable
3 - overdefined variable (need to be resolved)
*/
static int _resolve_subject_value(hir_subject_t* s, sym_table_t* smt, defined_variable_t* out) {
    if (HIR_is_vartype(s->t)) {
        variable_info_t vi;
        if (!VRTB_get_info_id(s->storage.var.v_id, &vi, &smt->v) || !vi.vdi.defined) return 0;
        else {
            out->const_value   = vi.vdi.definition;
            out->defined_value = vi.vdi.defined == DEFINED_VARIABLE ? 2 : 3;
        }
    }
    else {
        out->defined_value = 1;
        switch (HIR_is_defined_type(s->t)) {
            case 1: out->const_value = s->storage.num.value->to_llong(s->storage.num.value); break;
            case 2: out->const_value = s->storage.cnst.value; break;
            default: return 0;
        }
    }
    
    return 1;
}

static int _sparce_find_variable_define_location(hir_block_t* b, symbol_id_t v_id, trace_location_t* loc) {
    int found = 0;
    while (b) {
        if (found && b->op == HIR_SETPOS) {
            loc->line   = b->farg->storage.pos.line;
            loc->column = b->farg->storage.pos.column;
            loc->file   = b->farg->storage.pos.file;
            break;
        }
        else {
            if (
                b->farg && 
                HIR_is_vartype(b->farg->t) &&
                b->farg->storage.var.v_id == v_id
            ) found = 1;
        }

        b = b->prev;
    }

    return found;
}

static int _dereference_error(hir_block_t* hb, hir_subject_t* s, sym_table_t* smt, hir_visitors_ctx_t* ctx) {
    trace_location_t curr_loc = { 
        .column = ctx->curr_location.column, .file = ctx->curr_location.file, .line = ctx->curr_location.line 
    };

    defined_variable_t di;
    _resolve_subject_value(s, smt, &di);

    queue_t work_vars;
    queue_init(&work_vars);

    switch (di.defined_value) {
        /* Number defined */
        case 1: {
            if (!di.const_value) SEMANTIC_WARNING(" %s NULL-dereference error!", _format_location(&curr_loc));
            queue_free(&work_vars);
            return 0;
        }
        /* Variable defined */
        case 2: {
            if (!di.const_value) SEMANTIC_WARNING(
                " %s NULL-dereference error (variable '%s' is NULL)!", 
                _format_location(&curr_loc), _resolve_variable_name(s->storage.var.v_id, smt)
            );
            queue_free(&work_vars);
            return 0;
        }
        /* Overdefined */
        case 3: queue_push(&work_vars, (void*)di.const_value);
        default: break;
    }

    trace_t trace;
    TRACE_init_trace(&trace);

    int res = 1;
    symbol_id_t v_id;
    while (queue_pop(&work_vars, (void**)&v_id)) {
        list_t* possible_definitions;
        if (map_get(&ctx->definitions, (long)v_id, (void**)&possible_definitions)) {
            foreach (symbol_id_t p_id, possible_definitions) {
                queue_push(&work_vars, (void*)p_id);
            }
        }

        variable_info_t vi;
        if (!VRTB_get_info_id(v_id, &vi, &smt->v) || !vi.vdi.defined) continue;
        else {
            if (vi.vdi.defined == OVERDEFINED_VARIABLE && vi.vdi.definition != vi.v_id) queue_push(&work_vars, (void*)vi.vdi.definition);
            else {
                if (!vi.vdi.definition) {
                    res = 0;
                    trace_location_t loc;
                    _sparce_find_variable_define_location(hb, vi.v_id, &loc);
                    TRACE_add_location(&trace, &loc, "Variable '%s' becomes NULL-value", vi.name->body);
                }
            }
        }
    }

    if (!res) {
        trace_location_t loc = { .column = ctx->curr_location.column, .file = ctx->curr_location.file, .line = ctx->curr_location.line };
        TRACE_add_location(&trace, &loc, "Possible NULL-dereference error (variable '%s' is NULL)!", _resolve_variable_name(s->storage.var.v_id, smt));
    }

    _print_trace(&trace);

    queue_free(&work_vars);
    TRACE_unload_trace(&trace);
    return 1;
}

int HIRWLKR_visit_gdref_instruction(HIR_VISITOR_ARGS) {
    HIR_VISITOR_ARGS_USE;
    return _dereference_error(b, b->sarg, smt, ctx);
}

int HIRWLKR_visit_ldref_instruction(HIR_VISITOR_ARGS) {
    HIR_VISITOR_ARGS_USE;
    return _dereference_error(b, b->farg, smt, ctx);
}

int HIRWLKR_visit_ifop2_instruction(HIR_VISITOR_ARGS) {
    HIR_VISITOR_ARGS_USE;

    trace_location_t curr_loc = { 
        .column = ctx->curr_location.column, .file = ctx->curr_location.file, .line = ctx->curr_location.line 
    };

    defined_variable_t di;
    if (!_resolve_subject_value(b->farg, smt, &di)) {
        return 1;
    }

    switch (di.defined_value) {
        case 1: SEMANTIC_WARNING(" %s 'If' with a constant value '%s'!", _format_location(&curr_loc), di.const_value ? "true" : "false"); break;
        case 2: 
            SEMANTIC_WARNING(
                " %s 'If' with a constant value (variable '%s' is equals '%s')!", 
                _format_location(&curr_loc), _resolve_variable_name(b->farg->storage.var.v_id, smt), di.const_value ? "true" : "false"
            ); 
        break;
        default: break;
    }


    return 1;
}
