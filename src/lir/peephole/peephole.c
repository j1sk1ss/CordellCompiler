#include <lir/peephole/peephole.h>

static int _load_peephole_patterns(peephole_t* peephole) {
    return 1;
}

int LIR_peephole_optimization(cfg_ctx_t* cctx, peephole_t* peephole) {
    return peephole->perform_peephole(cctx);
}
