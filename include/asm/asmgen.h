#ifndef GENERATOR_H_
#define GENERATOR_H_

#include <lir/lir.h>
#include <lir/lirgen.h>
#include <asm/asmctx.h>

/*
ASM_generate function generates ASM code for target platform.
Params:
- lctx - LIR.
- g - Generator specific functions.
- output - Output file.

Return 1 if generation success.
Return 0 if something goes wrong.
*/
int ASM_generate(lir_ctx_t* lctx, asm_gen_t* g, FILE* output);

#endif