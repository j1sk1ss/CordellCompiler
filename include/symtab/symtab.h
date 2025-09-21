#ifndef SYMTAB_H_
#define SYMTAB_H_

#include <symtab/arrtb.h>
#include <symtab/vartb.h>
#include <symtab/functb.h>

typedef struct {
    vartab_ctx_t*  v;
    arrtab_ctx_t*  a;
    functab_ctx_t* f;
} sym_table_t;

#endif