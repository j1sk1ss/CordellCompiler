#include <lir/peephole/peephole.h>
// TODO: switch to a 'window' optimization
int LIR_peephole_optimization(cfg_ctx_t* cctx, peephole_t* peephole) {
    return peephole->perform_peephole(cctx);
}
