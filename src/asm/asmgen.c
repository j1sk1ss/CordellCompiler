#include <asm/asmgen.h>

gen_ctx_t* ASM_create_ctx() {
    gen_ctx_t* ctx = (gen_ctx_t*)mm_malloc(sizeof(gen_ctx_t));
    if (!ctx) return NULL;
    str_memset(ctx, 0, sizeof(gen_ctx_t));
    return ctx;
}

int ASM_destroy_ctx(gen_ctx_t* ctx) {
    if (!ctx) return 0;
    mm_free(ctx);
    return 1;
}

int ASM_generate(
    gen_ctx_t* ctx, int (*declarator)(ast_node_t*, FILE*), int (*generator)(ir_block_t*, FILE*), FILE* output
) {
    declarator(ctx->synt->r, output);
    generator(ctx->ir->h, output);
    return 1;
}
