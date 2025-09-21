#include <lir/x86_64_gnu_nasm/x86_64_lirgen.h>

int x86_64_generate_lir(hir_ctx_t* hctx, lir_ctx_t* ctx, sym_table_t* smt) {
    hir_block_t* h = hctx->h;
    while (h) {
        switch (h->op) {
            case HIR_FDCL: {
                LIR_BLOCK1(ctx, LIR_FDCL, LIR_SUBJ_FUNCNAME(h->farg));
                break;
            }

            default: break;
        }
        
        h = h->next;
    }

    return 1;
}
