#ifndef SSA_H_
#define SSA_H_

#include <std/mm.h>
#include <std/set.h>
#include <std/map.h>
#include <std/list.h>
#include <std/tuple.h>
#include <symtab/symtab.h>
#include <hir/hir.h>
#include <hir/hirgen.h>
#include <hir/cfg.h>

typedef struct {
    map_t vers;
} ssa_ctx_t;

/*
Insert the PHI placeholder for variables in the Dominance frontier.
Params:
    - `cctx` - CFG context.
    - `smt` - Symtable.

Returns 1 if operation succeed. Otherwise will return 0.
*/
int HIR_SSA_insert_phi(cfg_ctx_t* cctx, sym_table_t* smt);

/*
Apply SSA renaming to the CFG context.
Note: This function must be used only after the HIR_SSA_insert_phi function.
Note 2: This function will transform the input CFG. Be careful, this operation
        can't be revesed.
Params:
    - `cctx` - CFG context.
    - `ctx` - SSA context.
    - `smt` - Symtable.

Returns 1 if operation succeed. Otherwise will return 0.
*/
int HIR_SSA_rename(cfg_ctx_t* cctx, ssa_ctx_t* ctx, sym_table_t* smt);

#endif