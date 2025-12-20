#ifndef HIR_CONSTFOLD_H_
#define HIR_CONSTFOLD_H_

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

#endif