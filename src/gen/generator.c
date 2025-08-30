#include <generator.h>

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
