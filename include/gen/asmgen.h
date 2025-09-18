#ifndef GENERATOR_H_
#define GENERATOR_H_

#include <gen/genctx.h>

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