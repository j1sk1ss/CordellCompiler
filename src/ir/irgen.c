#include <ir/irgen.h>

int IR_generate(ir_gen_t* g, ir_ctx_t* ctx) {
    g->datagen(ctx->synt->r, EXT_SECTION, NO_BSS, g, ctx);
    IR_BLOCK1(ctx, RAW, IR_SUBJ_STR(8, "section .data"));
    g->datagen(ctx->synt->r, DATA_SECTION, NO_BSS, g, ctx);
    IR_BLOCK1(ctx, RAW, IR_SUBJ_STR(8, "section .rodata"));
    g->datagen(ctx->synt->r, RODATA_SECTION, NO_BSS, g, ctx);
    IR_BLOCK1(ctx, RAW, IR_SUBJ_STR(8, "section .bss"));
    g->datagen(ctx->synt->r, DATA_SECTION, BSS, g, ctx);
    IR_BLOCK1(ctx, RAW, IR_SUBJ_STR(8, "section .text"));
    g->funcdef(ctx->synt->r, g, ctx);
    scope_reset(&ctx->heap);
    g->blockgen(ctx->synt->r, g, ctx);
    return 1;
}
