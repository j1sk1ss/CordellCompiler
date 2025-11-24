#ifndef HIR_CONSTFOLD_H_
#define HIR_CONSTFOLD_H_

#include <hir/dag.h>

int HIR_sparse_const_propagation(dag_ctx_t* dctx, sym_table_t* smt);

#endif