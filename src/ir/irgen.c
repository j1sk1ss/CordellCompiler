#include <ir/irgen.h>

int IR_generate(ir_gen_t* g, ir_ctx_t* ctx) {
    // TODO:
    // g->datagen(ctx->synt->r, output, EXT_SECTION, NO_BSS, ctx, g);
    // iprintf(output, "section .data\n");
    // g->datagen(ctx->synt->r, output, DATA_SECTION, NO_BSS, ctx, g);
    // iprintf(output, "section .rodata\n");
    // g->datagen(ctx->synt->r, output, RODATA_SECTION, NO_BSS, ctx, g);
    // iprintf(output, "section .bss\n");
    // g->datagen(ctx->synt->r, output, DATA_SECTION, BSS, ctx, g);
    // iprintf(output, "section .text\n");
    g->funcdef(ctx->synt->r, g, ctx);
    scope_reset(&ctx->heap);
    g->blockgen(ctx->synt->r, g, ctx);
    return 1;
}
