#include <generator.h>

gen_ctx_t* GEN_create_ctx() {
    gen_ctx_t* ctx = (gen_ctx_t*)mm_malloc(sizeof(gen_ctx_t));
    if (!ctx) return NULL;
    str_memset(ctx, 0, sizeof(gen_ctx_t));
    return ctx;
}

int GEN_destroy_ctx(gen_ctx_t* ctx) {
    if (!ctx) return 0;
    mm_free(ctx);
    return 1;
}

int GEN_generate(gen_ctx_t* ctx, FILE* output) {
    iprintf(output, "section .data\n");
    ctx->datagen(ctx->synt->r, output, DATA_SECTION, NO_BSS, ctx);
    iprintf(output, "section .rodata\n");
    ctx->datagen(ctx->synt->r, output, RODATA_SECTION, NO_BSS, ctx);
    iprintf(output, "section .bss\n");
    ctx->datagen(ctx->synt->r, output, DATA_SECTION, BSS, ctx);

    iprintf(output, "section .text\n");
    ctx->funcdef(ctx->synt->r, output, ctx);
    ctx->blockgen(ctx->synt->r, output, ctx);
    return 1;
}
