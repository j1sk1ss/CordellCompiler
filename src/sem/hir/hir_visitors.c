#include <sem/hir/hir_visitors.h>

static inline char* _format_location(hir_visitors_ctx_t* ctx) {
    static char buff[256] = { 0 };
    if (ctx->curr_location.file) snprintf(buff, sizeof(buff), "[%s:%li:%li]", ctx->curr_location.file->body, ctx->curr_location.line, ctx->curr_location.column);
    else snprintf(buff, sizeof(buff), "[%li:%li]", ctx->curr_location.line, ctx->curr_location.column);
    return buff;
}

static const char* _resolve_variable_name(symbol_id_t id, sym_table_t* smt) {
    variable_info_t vi;
    symbol_id_t p_id = NO_SYMBOL_ID;
    do {
        if (VRTB_get_info_id(id, &vi, &smt->v)) p_id = vi.p_id;
    } while (p_id != NO_SYMBOL_ID);
    return vi.name->body;
}

int HIRWLKR_visit_setpos_instruction(HIR_VISITOR_ARGS) {
    HIR_VISITOR_ARGS_USE;
    str_memcpy(&ctx->prev_location, &ctx->curr_location, sizeof(ctx->curr_location));
    ctx->curr_location.column = b->farg->storage.pos.column;
    ctx->curr_location.line   = b->farg->storage.pos.line;
    ctx->curr_location.file   = b->farg->storage.pos.file;
    return 1;
}

typedef struct {
    long const_value;
    char defined_value;
} defined_variable_t;

static int _resolve_subject_value(hir_subject_t* s, sym_table_t* smt, defined_variable_t* out) {
    if (HIR_is_vartype(s->t)) {
        variable_info_t vi;
        if (!VRTB_get_info_id(s->storage.var.v_id, &vi, &smt->v) || !vi.vdi.defined) return 0;
        else {
            out->const_value = vi.vdi.definition;
            out->defined_value = 2;
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

static int _dereference_error(hir_subject_t* s, sym_table_t* smt, hir_visitors_ctx_t* ctx) {
    defined_variable_t di;
    if (!_resolve_subject_value(s, smt, &di)) {
        return 1;
    }

    switch (di.defined_value) {
        case 1:
            if (!di.const_value) SEMANTIC_WARNING(" %s NULL-dereference error!", _format_location(ctx));
        return 0;
        case 2:
            if (!di.const_value) SEMANTIC_WARNING(
                " %s NULL-dereference error (variable '%s' is NULL)!", 
                _format_location(ctx), _resolve_variable_name(s->storage.var.v_id, smt)
            ); 
        return 0;
        default: break;
    }

    return 1;
}

int HIRWLKR_visit_gdref_instruction(HIR_VISITOR_ARGS) {
    HIR_VISITOR_ARGS_USE;
    return _dereference_error(b->sarg, smt, ctx);
}

int HIRWLKR_visit_ldref_instruction(HIR_VISITOR_ARGS) {
    HIR_VISITOR_ARGS_USE;
    return _dereference_error(b->farg, smt, ctx);
}

int HIRWLKR_visit_ifop2_instruction(HIR_VISITOR_ARGS) {
    HIR_VISITOR_ARGS_USE;

    defined_variable_t di;
    if (!_resolve_subject_value(b->farg, smt, &di)) {
        return 1;
    }

    switch (di.defined_value) {
        case 1: SEMANTIC_WARNING(" %s 'If' with a constant value '%s'!", _format_location(ctx), di.const_value ? "true" : "false"); break;
        case 2: 
            SEMANTIC_WARNING(
                " %s 'If' with a constant value (variable '%s' is equals '%s')!", 
                _format_location(ctx), _resolve_variable_name(b->farg->storage.var.v_id, smt), di.const_value ? "true" : "false"
            ); 
        break;
        default: break;
    }


    return 1;
}
