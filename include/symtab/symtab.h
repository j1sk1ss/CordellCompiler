#ifndef SYMTAB_H_
#define SYMTAB_H_

#include <std/mm.h>
#include <symtab/arrtb.h>
#include <symtab/vartb.h>
#include <symtab/functb.h>
#include <symtab/strtb.h>
#include <symtab/alliasmap.h>

typedef struct {
    vartab_ctx_t  v; /* Variables' symtable */
    arrtab_ctx_t  a; /* Arrays' symtable    */
    functab_ctx_t f; /* Functions' symtable */
    strtb_ctx_t   s; /* Strings' symtable   */
    allias_map_t  m; /* Alliases' symtable  */
} sym_table_t;

/*
Init all maps in the symtable.
Params:
    - `smt` - Symtable.

Returns 1 if succeeds.
*/
int SMT_init(sym_table_t* smt);

/*
Compres the next parts of the symtable:
    - Variables' symtable
    - Functions' symtable
    - Strings' symtable

Params:
    - `smt` - Symtable.

Returns 1 if succeeds.
*/
int SMT_compress(sym_table_t* smt);

/*
Unload the symtable.
Params:
    - `smt` - Symtable.

Returns 1 if succeeds.
*/
int SMT_unload(sym_table_t* smt);

#endif