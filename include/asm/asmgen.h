#ifndef GENERATOR_H_
#define GENERATOR_H_

#include <ir/ir.h>
#include <ir/irctx.h>
#include <ast/syntax.h>

typedef struct gen_ctx {
    syntax_ctx_t* synt;
    ir_ctx_t*     ir;
} gen_ctx_t;

/*
Create generator context in heap.
Return pointer to allocated context.
*/
gen_ctx_t* ASM_create_ctx();

/*
Unload generater that was allocated in heap.
Params:
- ctx - Pointer to generator context.

Return 1 if free success.
Return 0 if something goes wrong.
*/
int ASM_destroy_ctx(gen_ctx_t* ctx);

/*
ASM_generate function generates ASM code for target platform.
Params:
- root - AST tree root.
- output - Output file.

Return 1 if generation success.
Return 0 if something goes wrong.
*/
int ASM_generate(
    gen_ctx_t* ctx, int (*declarator)(ast_node_t*, FILE*), int (*generator)(ir_block_t*, FILE*), FILE* output
);

#endif