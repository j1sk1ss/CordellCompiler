#ifndef SSA_H_
#define SSA_H_

#include <std/mm.h>
#include <std/stack.h>
#include <std/set.h>
#include <std/map.h>
#include <std/list.h>
#include <std/tuple.h>
#include <symtab/symtab.h>
#include <hir/hir.h>
#include <hir/cfg.h>

typedef struct {
    long curr_id;
    long v_id;
} varver_t;

typedef struct {
    map_t vers;
} ssa_ctx_t;

int HIR_SSA_rename(cfg_ctx_t* cctx, ssa_ctx_t* ctx, sym_table_t* smt);
int HIR_SSA_insert_phi(cfg_ctx_t* cctx, sym_table_t* smt);

#endif