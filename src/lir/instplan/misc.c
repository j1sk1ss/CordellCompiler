#include <lir/instplan/instplan.h>

lir_block_t* LIR_planner_get_next_func_abi(lir_block_t* entry, lir_block_t* exit, int offset) {
    if (entry == exit) return NULL;
    while (entry && entry->op != LIR_FCLL && entry->op != LIR_ECLL) {
        if (entry->farg && entry->op == LIR_STFARG && offset-- <= 0) return entry;
        if (entry == exit) break;
        entry = entry->prev;
    }

    return NULL;
}

lir_block_t* LIR_planner_get_next_sysc_abi(lir_block_t* entry, lir_block_t* exit, int offset) {
    if (entry == exit) return NULL;
    while (entry && entry->op != LIR_SYSC) {
        if (entry->op == LIR_STSARG && offset-- <= 0) return entry;
        if (entry == exit) break;
        entry = entry->prev;
    }

    return NULL;
}

lir_block_t* LIR_planner_get_func_res(lir_block_t* fn, lir_block_t* exit) {
    if (fn == exit) return NULL;
    lir_block_t* candidate = fn->next;
    if (candidate->op == LIR_LOADFRET) {
        return candidate;
    }

    return NULL;
}
