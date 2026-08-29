#ifndef Z3_OPT_H_
#define Z3_OPT_H_

#include <symtab/symtab.h>
#include <hir/hir.h>
#include <hir/hirgen.h>
#include <hir/z3_wrapper.h>

int Z3OPT_deadbranch(cfg_ctx_t* cctx, sym_table_t* smt);

#endif