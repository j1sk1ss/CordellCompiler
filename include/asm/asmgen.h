#ifndef GENERATOR_H_
#define GENERATOR_H_

#include <asm/genctx.h>

typedef struct gen_ctx {
    short         label;
    scope_stack_t heap;
    syntax_ctx_t* synt;
} gen_ctx_t;

gen_ctx_t* GEN_create_ctx();
int GEN_destroy_ctx(gen_ctx_t* ctx);

/*
GEN_generate function generates ASM code for target platform.
Params:
- root - AST tree root.
- output - Output file.

Return 1 if generation success.
Return 0 if something goes wrong.
*/
int GEN_generate(gen_ctx_t* ctx, gen_t* g, FILE* output);

#endif