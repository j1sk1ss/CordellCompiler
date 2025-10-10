#ifndef DFG_H_
#define DFG_H_

#include <std/set.h>
#include <symtab/symtab.h>
#include <hir/cfg.h>

int HIR_DFG_collect_defs(cfg_ctx_t* cctx);
int HIR_DFG_collect_uses(cfg_ctx_t* cctx);
int HIR_DFG_compute_inout(cfg_ctx_t* cctx);
int HIR_DFG_create_deall(cfg_ctx_t* cctx, sym_table_t* smt);

#endif