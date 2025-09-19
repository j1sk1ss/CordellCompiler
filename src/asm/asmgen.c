#include <asm/asmgen.h>

asmgen_ctx_t* ASM_create_ctx() {
    asmgen_ctx_t* ctx = (asmgen_ctx_t*)mm_malloc(sizeof(asmgen_ctx_t));
    if (!ctx) return NULL;
    str_memset(ctx, 0, sizeof(asmgen_ctx_t));
    return ctx;
}

int ASM_destroy_ctx(asmgen_ctx_t* ctx) {
    if (!ctx) return 0;
    mm_free(ctx);
    return 1;
}

int ASM_generate(asmgen_ctx_t* ctx, asm_gen_t* g, FILE* output) {
    g->declarator(ctx->synt->r, output);
    g->generator(ctx->ir->h, output);
    return 1;
}
