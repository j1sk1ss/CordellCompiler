#include <lir/x86_64_gnu_nasm/x86_64_lirgen.h>

int x86_64_generate_ref(lir_ctx_t* ctx, hir_block_t* h, sym_table_t* smt) {
    switch (h->op) {
        case HIR_REF: {
            x86_64_store_var_reg(LIR_REF, ctx, h->sarg, RAX, DEFAULT_TYPE_SIZE, smt);
            x86_64_load_var_reg(LIR_iMOV, ctx, h->farg, RAX, -1, smt);
            break;
        }

        case HIR_GDREF: {
            x86_64_store_var_reg(LIR_GDREF, ctx, h->sarg, RAX, DEFAULT_TYPE_SIZE, smt);
            x86_64_load_var_reg(LIR_iMOV, ctx, h->farg, RAX, -1, smt);
            break;
        }

        case HIR_LDREF: {
            x86_64_store_var_reg(LIR_iMOV, ctx, h->farg, RAX, -1, smt);
            x86_64_store_var_reg(LIR_LDREF, ctx, h->sarg, RAX, DEFAULT_TYPE_SIZE, smt);
            break;
        }
    }

    return 1;
}
