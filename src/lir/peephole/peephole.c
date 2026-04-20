#include <lir/peephole/peephole.h>
// TODO: If we have a pop instruction, we can remove push/pop, if the register is re-written

lir_block_t* LIR_get_back_instruction(lir_block_t* c, lir_block_t* exit, int skip) {
    if (!skip) return c;
    if (c == exit) return NULL;
    for (; c && c != exit; c = c->prev) {
        if (c->unused || c->op == LIR_BB || c->op == LIR_VRDEALL) continue;
        if ((c == exit) || skip-- == 0) return c;
    }

    return NULL;
}

lir_block_t* LIR_get_near_instruction(lir_block_t* c, lir_block_t* exit, int skip) {
    if (!skip) return c;
    if (c == exit) return NULL;
    for (; c && c != exit; c = c->next) {
        if (c->unused || c->op == LIR_BB || c->op == LIR_VRDEALL) continue;
        if ((c == exit) || skip-- == 0) return c;
    }

    return NULL;
}

long LIR_peephole_get_long_number(lir_subject_t* s) {
    switch (s->t) {
        case LIR_CONSTVAL: return s->storage.cnst.value;
        case LIR_NUMBER:   return s->storage.num.value->to_llong(s->storage.num.value);
        default: return -1;
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
    foreach (cfg_func_t* fb, &cctx->funcs) {
        if (!fb->used) continue;
        foreach (cfg_block_t* bb, &fb->blocks) {
            int optimized = 0;
            do {
                optimized = peephole_first_pass(bb);
                peephole->perform_peephole(cctx);
            } while (optimized);
        }
    }

    return 1;
}
