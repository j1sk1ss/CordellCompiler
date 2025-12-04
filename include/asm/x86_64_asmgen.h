#ifndef X86_64_ASM_GENERATOR_H_
#define X86_64_ASM_GENERATOR_H_

#include <std/mem.h>
#include <std/str.h>
#include <std/math.h>
#include <prep/token_types.h>
#include <symtab/symtab.h>
#include <hir/hir.h>
#include <lir/lir.h>

/*
Main generator script. Generation based on linear LIR instead of LIR CFG.
Params:
- h - LIR.
- smt - Symtable.
- output - Output location for generated ASM.

Return 1 if success, otherwise - 0.
*/
int x86_64_generate_asm(lir_ctx_t* h, sym_table_t* smt, FILE* output);

/*
Generate global data in ASM file header.
- smt - Symtable.
- output - Output location for generated ASM.

Return 1 if success, otherwise - 0.
*/
int x86_64_generate_data(sym_table_t* smt, FILE* output);

/*
Generate function stackframe (open).
Params:
- h - Function LIR head.
- end - Function end (FEND / STEND).
- output - Output location for generated ASM.

Return 1 if success, otherwise - 0.
*/
int x86_64_generate_stackframe(lir_block_t* h, lir_operation_t end, FILE* output);

/*
Generate function stackframe (close).
Params:
- output - Output location for generated ASM.

Return 1 if success, otherwise - 0.
*/
int x86_64_kill_stackframe(FILE* output);

/*
Format input subject and return char*.
Params:
- v - LIR subject for formatting.
- smt - Symtable.

Return 1 if success, otherwise - 0.
*/
const char* x86_64_asm_variable(lir_subject_t* v, sym_table_t* smt);

#endif