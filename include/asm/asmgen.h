#ifndef GENERATOR_H_
#define GENERATOR_H_

#include <ir/ir.h>
#include <ir/irctx.h>
#include <ast/syntax.h>
#include <asm/asmctx.h>

typedef struct gen_ctx {
    syntax_ctx_t* synt;
    ir_ctx_t*     ir;
} asmgen_ctx_t;

/*
Create generator context in heap.
Return pointer to allocated context.
*/
asmgen_ctx_t* ASM_create_ctx();

/*
Unload generater that was allocated in heap.
Params:
- ctx - Pointer to generator context.

Return 1 if free success.
Return 0 if something goes wrong.
*/
int ASM_destroy_ctx(asmgen_ctx_t* ctx);

/*
ASM_generate function generates ASM code for target platform.
Params:
- ctx - Generator context.
- g - Generator specific functions.
- output - Output file.

Return 1 if generation success.
Return 0 if something goes wrong.
*/
int ASM_generate(asmgen_ctx_t* ctx, asm_gen_t* g, FILE* output);

#endif