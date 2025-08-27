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
    x86_64_generate_data(ctx->synt->r, output, DATA_SECTION, NO_BSS);
    iprintf(output, "section .rodata\n");
    x86_64_generate_data(ctx->synt->r, output, RODATA_SECTION, NO_BSS);
    iprintf(output, "section .bss\n");
    x86_64_generate_data(ctx->synt->r, output, DATA_SECTION, BSS);

    iprintf(output, "section .text\n");
    x86_64_generate_funcdef(ctx->synt->r, output, ctx);
    x86_64_generate_block(ctx->synt->r, output, ctx);
    return 1;
}
