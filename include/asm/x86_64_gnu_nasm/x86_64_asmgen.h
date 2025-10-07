#ifndef X86_64_ASM_GENERATOR_H_
#define X86_64_ASM_GENERATOR_H_

#include <std/math.h>
#include <prep/token_types.h>
#include <symtab/symtab.h>
#include <lir/lir.h>

int x86_64_generate_asm(lir_ctx_t* h, sym_table_t* smt, FILE* output);
int x86_64_generate_data(sym_table_t* smt, FILE* output);
int x86_64_generate_stackframe(lir_block_t* h, lir_operation_t end, FILE* output);
int x86_64_kill_stackframe(FILE* output);
const char* x86_64_asm_variable(lir_subject_t* v, sym_table_t* smt);

#endif