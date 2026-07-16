#include <symtab/scopetb.h>

symbol_id_t SCPTB_push_scope(scopetab_ctx_t* ctx, sstack_t* stack) {
    if (!ctx || !stack) return NO_SYMBOL_ID;

    symbol_id_t parent = NO_SYMBOL_ID;
    stack_top(stack, (void**)&parent);

    symbol_id_t id = ++ctx->curr_id;
    if (!map_put(&ctx->parents, id, (void*)((long)parent))) return NO_SYMBOL_ID;
    if (!stack_push(stack, (void*)((long)id))) {
        map_remove(&ctx->parents, id);
        return NO_SYMBOL_ID;
    }

    return id;
}

symbol_id_t SCPTB_get_parent(symbol_id_t id, scopetab_ctx_t* ctx) {
    if (!ctx || id == NO_SYMBOL_ID) return 0;
    symbol_id_t value = NO_SYMBOL_ID;
    if (!map_get(&ctx->parents, id, (void**)&value)) return NO_SYMBOL_ID;
    return value;
}

int SCPTB_is_visible(symbol_id_t target, symbol_id_t from, scopetab_ctx_t* ctx) {
    if (!ctx || target == NO_SYMBOL_ID || from == NO_SYMBOL_ID) return 0;
    for (symbol_id_t s = from; s != NO_SYMBOL_ID; s = SCPTB_get_parent(s, ctx)) {
        if (s == target) return 1;
    }

    return 0;
}

int SCPTB_unload(scopetab_ctx_t* ctx) {
    if (!ctx) return 0;
    map_free(&ctx->parents);
    ctx->curr_id = 0;
    return 1;
}
