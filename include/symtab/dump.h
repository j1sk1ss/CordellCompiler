#ifndef SMT_DUMP_H_
#define SMT_DUMP_H_

#include <stdio.h>
#include <stdarg.h>
#include <string.h>
#include <symtab/symtab.h>

int DUMP_format_vartb(sym_table_t* smt, FILE* output);
int DUMP_format_fntb(sym_table_t* smt, FILE* output);
int DUMP_format_sectb(sym_table_t* smt, FILE* output);

#endif
