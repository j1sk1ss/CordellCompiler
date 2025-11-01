#include <lir/lirgen.h>

int LIR_generate(cfg_ctx_t* cctx, lir_ctx_t* ctx, sym_table_t* smt) {
    return LIR_generate_block(cctx, ctx, smt);
}
