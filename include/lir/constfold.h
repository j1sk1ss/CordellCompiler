#ifndef LIR_CONSTFOLD_H_
#define LIR_CONSTFOLD_H_

#include <hir/cfg.h>
#include <lir/lir.h>
#include <lir/lirgen.h>
#include <lir/lir_types.h>
#include <symtab/symtab.h>

int LIR_apply_sparse_const_propagation(cfg_ctx_t* cctx, sym_table_t* smt);

#endif