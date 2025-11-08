#ifndef GENERATOR_H_
#define GENERATOR_H_

#include <symtab/symtab.h>
#include <hir/cfg.h>
#include <lir/lir.h>

typedef struct {
    int (*generator)(lir_ctx_t*, sym_table_t*, FILE*);
} asm_gen_t;

/*
ASM_generate function generates ASM code for target platform.
Params:
- lctx - LIR.
- g - Generator specific functions.
- output - Output file.

Return 1 if generation success.
Return 0 if something goes wrong.
*/
int ASM_generate(lir_ctx_t* lctx, sym_table_t* smt, asm_gen_t* g, FILE* output);

#endif