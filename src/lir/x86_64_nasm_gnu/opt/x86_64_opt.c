#include <lir/x86_64_gnu_nasm/opt/x86_64_opt.h>

int x86_64_optimization(lir_ctx_t* ctx) {
    regmap_t rmap;
    map_init(&rmap.regmap);

    lir_block_t* h = ctx->h;
    while (h) {
        switch (h->op) {
            case LIR_iMOV: {
                if (h->farg->t != LIR_REGISTER) break;
                int dstreg = h->farg->storage.reg.reg;
                if (h->sarg->t == LIR_REGISTER) {
                    REGMAP_update_storage(dstreg, REGMAP_create_value(REGISTER, h->sarg->storage.reg.reg), &rmap);
                }
                
                break;
            }
        }

        h = h->next;
    }

    return 1;
}
