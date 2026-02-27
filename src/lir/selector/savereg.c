#include <lir/selector/savereg.h>

int LIR_save_registers(cfg_ctx_t* cctx, register_saver_t* selector) {
    return selector->save_registers(cctx);
}
