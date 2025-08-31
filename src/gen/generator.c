#include <generator.h>

int GEN_generate(gen_ctx_t* ctx, gen_t* g, FILE* output) {
    iprintf(output, "section .data\n");
    g->datagen(ctx->synt->r, output, DATA_SECTION, NO_BSS, ctx, g);
    iprintf(output, "section .rodata\n");
    g->datagen(ctx->synt->r, output, RODATA_SECTION, NO_BSS, ctx, g);
    iprintf(output, "section .bss\n");
    g->datagen(ctx->synt->r, output, DATA_SECTION, BSS, ctx, g);

    iprintf(output, "section .text\n");
    g->funcdef(ctx->synt->r, output, ctx, g);
    g->blockgen(ctx->synt->r, output, ctx, g);
    return 1;
}
