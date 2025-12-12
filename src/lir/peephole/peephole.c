#include <lir/peephole/peephole.h>

long LIR_peephole_get_long_number(lir_subject_t* s) {
    switch (s->t) {
        case LIR_CONSTVAL: return s->storage.cnst.value;
        case LIR_NUMBER:   return s->storage.num.value->to_llong(s->storage.num.value);
        default: return 0;
    }
}

long LIR_peephole_get_sqrt_number(lir_subject_t* s) {
    long v = LIR_peephole_get_long_number(s);
    if (v < 0) return 0;
    return mth_sqrt((double)v);
}

long LIR_peephole_get_log2_number(lir_subject_t* s) {
    long v = LIR_peephole_get_long_number(s);
    if (v <= 0) return 0;
    return mth_log2((double)v);
}

int LIR_peephole_optimization(cfg_ctx_t* cctx, peephole_t* peephole) {
    foreach(cfg_func_t* fb, &cctx->funcs) {
        if (!fb->used) continue;
        foreach(cfg_block_t* bb, &fb->blocks) {
            peephole_first_pass(bb);
        }
    }

    return peephole->perform_peephole(cctx);
}
