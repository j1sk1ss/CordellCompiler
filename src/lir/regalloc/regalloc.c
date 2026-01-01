#include <lir/regalloc/regalloc.h>

int LIR_regalloc(cfg_ctx_t* cctx, sym_table_t* smt, map_t* colors, regalloc_t* allocator) {
    return allocator->regallocate(cctx, smt, colors);
}

int LIR_apply_regalloc(sym_table_t* smt, map_t* colors) {
    map_foreach (variable_info_t* vi, &smt->v.vartb) {
        long reg;
        if (map_get(colors, vi->v_id, (void**)&reg)) {
            vi->vmi.reg = reg;
            vi->vmi.allocated = 1;
        }
    }

    return 1;
}
