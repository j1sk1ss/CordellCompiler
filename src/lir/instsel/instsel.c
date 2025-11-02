#include <lir/instsel/instsel.h>

int LIR_select_instructions(cfg_ctx_t* cctx, sym_table_t* smt, inst_selector_h* selector) {
    return selector->select_instructions(cctx, smt);
}
