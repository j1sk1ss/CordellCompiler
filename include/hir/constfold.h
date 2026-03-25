#ifndef HIR_CONSTFOLD_H_
#define HIR_CONSTFOLD_H_

#include <std/mm.h>
#include <std/map.h>
#include <std/list.h>
#include <hir/cfg.h>
#include <hir/dag.h>

/*
Fill define slot in the symtable according to the DAG context.
Note: This function doesn't change the code.
Params:
    - `dctx` - DAG context.
    - `smt` - Symtable.

Return 1 if success, otherwise 0.
*/
int HIR_sparse_const_propagation(dag_ctx_t* dctx, sym_table_t* smt);

/*
Update define slots for loaded arguments in functions. It will collect all
function calls, and if same function calls have same arguments (and these
arguments are defined), it will define them as a value in the function which
is called.
Params:
    - `cctx` - CFG context.
    - `smt` - Symtable.

Returns 1 if succeeds and has changed something.
*/
int HIR_sparse_const_funcall_propagation(cfg_ctx_t* cctx, sym_table_t* smt);

/*
Update define slots of return subjects of function calls, which return values
are defined.
Params:
    - `cctx` - CFG context.
    - `smt` - Symtable.

Returns 1 if succeeds and has changed something.
*/
int HIR_sparce_const_fret_propagation(cfg_ctx_t* cctx, sym_table_t* smt);

#endif