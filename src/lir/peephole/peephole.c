#include <lir/peephole/peephole.h>

int LIR_peephole_optimization(cfg_ctx_t* cctx, sym_table_t* smt, peephole_t* peephole) {
    return peephole->perform_peephole(cctx, smt);
}
