#ifndef DFG_H_
#define DFG_H_

#include <std/set.h>
#include <std/list.h>
#include <std/stack.h>
#include <symtab/symtab.h>
#include <hir/cfg.h>
#include <lir/lir.h>
#include <lir/lirgen.h>

/*
Compute USE and DEF sets. The idea is to collect 
all uses before defines. Also, if there is an already
defined variable, it can't be used.
Params:
    - `cctx` - CFG context.

Returns 1 if succeeds.
*/
int LIR_DFG_compute_usedef(cfg_ctx_t* cctx);

/*
Compute IN and OUT sets.
```
IN  = union(USE, (OUT - DEF))
OUT = union(IN.successors)
```
Params:
    - `cctx` - CFG context.

Returns 1 if succeeds.
*/
int LIR_DFG_compute_inout(cfg_ctx_t* cctx);

/*
Put deallocate commands where needed.
Params:
    - `cctx` - CFG context.
    - `smt` - Symtable.

Returns 1 if succeeds.
*/
int LIR_DFG_create_deall(cfg_ctx_t* cctx, sym_table_t* smt);

/*
Sort phi-move groups according to assigned register colors to avoid clobbering
source registers before they are copied.
Params:
    - `cctx` - CFG context.
    - `colors` - Map from variable ids to allocated registers.

Returns 1 on success, otherwise 0.
*/
int LIR_RA_sort_phi_movs(cfg_ctx_t* cctx, map_t* colors);

#endif
