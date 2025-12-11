#include <lir/peephole/peephole.h>

int LIR_peephole_optimization(cfg_ctx_t* cctx, peephole_t* peephole) {
    foreach(cfg_func_t* fb, &cctx->funcs) {
        if (!fb->used) continue;
        foreach(cfg_block_t* bb, &fb->blocks) {
            peephole_first_pass(bb);
        }
    }

    return peephole->perform_peephole(cctx);
}
