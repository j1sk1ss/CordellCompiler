#include <lir/regalloc/regalloc.h>

int LIR_regalloc(cfg_ctx_t* cctx, sym_table_t* smt, regalloc_t* allocator) {
    return allocator->regallocate(cctx, smt);
}
