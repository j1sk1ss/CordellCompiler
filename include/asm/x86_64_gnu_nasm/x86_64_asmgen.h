#ifndef X86_64_ASM_GENERATOR_H_
#define X86_64_ASM_GENERATOR_H_

#include <symtab/symtab.h>
#include <lir/lir.h>

const char* x86_64_asm_variable(lir_subject_t* v, sym_table_t* smt);
int x86_64_generate_asm(lir_block_t* h, sym_table_t* smt, FILE* output);
int x86_64_generate_data(sym_table_t* smt, FILE* output);

#endif