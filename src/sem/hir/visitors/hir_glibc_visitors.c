#include <sem/hir/hir_visitors.h>

static inline func_info_t _get_finfo_from_call(hir_block_t* b, sym_table_t* smt) {
    if (b->op == HIR_SYSC || b->op == HIR_STORE_SYSC) return (func_info_t){ .id = NO_SYMBOL_ID };
    func_info_t fi;
    if (!FNTB_get_info_id(b->sarg->storage.str.s_id, &fi, &smt->f)) {
        return (func_info_t){ .id = NO_SYMBOL_ID };
    }

    return fi;
}

int HIRWLKR_null_free(HIR_VISITOR_ARGS) {
    HIR_VISITOR_ARGS_USE;
    func_info_t fi = _get_finfo_from_call(b, smt);
    if (
        fi.id == NO_SYMBOL_ID || !fi.name->requals(fi.name, "free") || 
        (list_size(&b->targ->storage.list.h) > 1)
    ) return 1;
    hir_subject_t* ptr = list_get_head(&b->targ->storage.list.h);
    if (ptr && !HIR_SEM_check_subject_value_and_provide_trace(b, bb, ptr, smt, ctx, 0, "NULL free argument error")) {
        trace_t trace;
        TRACE_init_trace(&trace);
        TRACE_add_location(&trace, &ctx->curr_location, "'free' function must accept non-NULL pointer to an allocation!");
        TRACE_print_and_free_trace(&trace);
    }

    return 1;
}

int HIRWLKR_zero_malloc(HIR_VISITOR_ARGS) {
    HIR_VISITOR_ARGS_USE;
    func_info_t fi = _get_finfo_from_call(b, smt);
    if (
        fi.id == NO_SYMBOL_ID || !fi.name->requals(fi.name, "malloc") || 
        (list_size(&b->targ->storage.list.h) > 1)
    ) return 1;
    hir_subject_t* ptr = list_get_head(&b->targ->storage.list.h);
    if (ptr && !HIR_SEM_check_subject_value_and_provide_trace(b, bb, ptr, smt, ctx, 0, "Zero malloc size error")) {
        trace_t trace;
        TRACE_init_trace(&trace);
        TRACE_add_location(&trace, &ctx->curr_location, "'malloc' function must accept non-Zero size!");
        TRACE_print_and_free_trace(&trace);
    } 

    return 1;
}
