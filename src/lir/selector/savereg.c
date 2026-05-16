#include <lir/selector/savereg.h>

int LIR_save_registers(cfg_ctx_t* cctx, call_graph_t* calls, sym_table_t* smt, register_saver_t* selector) {
    return selector->save_registers(cctx, calls, smt);
}
