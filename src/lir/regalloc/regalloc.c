#include <lir/regalloc/regalloc.h>

int LIR_regalloc(cfg_ctx_t* cctx, sym_table_t* smt, map_t* colors, regalloc_t* allocator) {
    return allocator->regallocate(cctx, smt, colors);
}

int LIR_apply_regalloc(sym_table_t* smt, map_t* colors) {
    map_foreach (variable_info_t* vi, &smt->v.vartb) {
        long reg;
        if (map_get(colors, vi->v_id, (void**)&reg)) {
            VRTB_update_memory(vi->v_id, FIELD_NO_CHANGE, FIELD_NO_CHANGE, reg, FIELD_NO_CHANGE, &smt->v);
        }
    }

    return 1;
}
