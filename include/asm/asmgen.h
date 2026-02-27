#ifndef GENERATOR_H_
#define GENERATOR_H_

#include <symtab/symtab.h>
#include <hir/cfg.h>
#include <lir/lir.h>

typedef struct {
    int (*generator)(cfg_ctx_t*, sym_table_t*, FILE*);
} asm_gen_t;

/*
ASM_generate function generates ASM code for target platform.
Params:
- `cctx` - CFG context.
- `g` - Generator specific functions.
- `output` - Output file.

Returns 1 if the generation was success.
Returns 0 if something went wrong.
*/
int ASM_generate(cfg_ctx_t* cctx, sym_table_t* smt, asm_gen_t* g, FILE* output);

#endif