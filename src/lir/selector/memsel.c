#include <lir/selector/memsel.h>

int LIR_select_memory(cfg_ctx_t* cctx, map_t* colors, sym_table_t* smt, mem_selector_h* selector) {
    return selector->select_memory(cctx, colors, smt);
}