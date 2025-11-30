#include <lir/regalloc/regalloc.h>

int LIR_regalloc(cfg_ctx_t* cctx, sym_table_t* smt, map_t* colors, regalloc_t* allocator) {
    return allocator->regallocate(cctx, smt, colors);
}

int LIR_apply_regalloc(sym_table_t* smt, map_t* colors) {
    map_iter_t it;
    variable_info_t* vi;
    map_iter_init(&smt->v.vartb, &it);
    while (map_iter_next(&it, (void**)&vi)) {
        long reg;
        if (map_get(colors, vi->v_id, (void**)&reg)) {
            vi->vmi.reg = reg;
            vi->vmi.allocated = 1;
        }
    }

    return 1;
}
