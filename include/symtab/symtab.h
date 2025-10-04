#ifndef SYMTAB_H_
#define SYMTAB_H_

#include <std/mm.h>
#include <symtab/arrtb.h>
#include <symtab/vartb.h>
#include <symtab/functb.h>
#include <symtab/strtb.h>
#include <symtab/alliasmap.h>

typedef struct {
    vartab_ctx_t  v;
    arrtab_ctx_t  a;
    functab_ctx_t f;
    strtb_ctx_t   s;
    allias_map_t  m;
} sym_table_t;

sym_table_t* SMT_create();
int SMT_init(sym_table_t* smt);
int SMT_unload(sym_table_t* smt);

#endif