#ifndef X86_64_ASM_GENERATOR_H_
#define X86_64_ASM_GENERATOR_H_

#include <std/mem.h>
#include <std/str.h>
#include <std/math.h>
#include <prep/token_types.h>
#include <symtab/symtab.h>
#include <hir/hir.h>
#include <hir/cfg.h>
#include <lir/lir.h>
#include <lir/lirgen.h>

#define EMIT_COMMAND(cmd, ...) fprintf(output, cmd "\n", ##__VA_ARGS__)

/*
Main generator script. Generation based on linear LIR instead of LIR CFG.
Params:
- `cctx` - CFG context.
- `smt` - Symtable.
- `output` - Output location for generated ASM.

Return 1 if success, otherwise - 0.
*/
int x86_64_generate_asm(cfg_ctx_t* cctx, sym_table_t* smt, FILE* output);

/*
Format input subject and return char*.
Params:
- `v` - LIR subject for formatting.
- `smt` - Symtable.

Return 1 if success, otherwise - 0.
*/
const char* format_lir_subject(lir_subject_t* v, sym_table_t* smt);

#endif