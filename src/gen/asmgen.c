#include <gen/asmgen.h>

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

int GEN_generate(gen_ctx_t* ctx, gen_t* g, FILE* output) {
    g->datagen(ctx->synt->r, output, EXT_SECTION, NO_BSS, ctx, g);
    iprintf(output, "section .data\n");
    g->datagen(ctx->synt->r, output, DATA_SECTION, NO_BSS, ctx, g);
    iprintf(output, "section .rodata\n");
    g->datagen(ctx->synt->r, output, RODATA_SECTION, NO_BSS, ctx, g);
    iprintf(output, "section .bss\n");
    g->datagen(ctx->synt->r, output, DATA_SECTION, BSS, ctx, g);

    iprintf(output, "section .text\n");
    g->funcdef(ctx->synt->r, output, ctx, g);

    scope_reset(&ctx->heap);
    g->blockgen(ctx->synt->r, output, ctx, g);
    return 1;
}
