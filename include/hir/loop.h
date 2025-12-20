#ifndef LOOP_H_
#define LOOP_H_

#include <std/mm.h>
#include <std/set.h>
#include <std/logg.h>
#include <std/list.h>
#include <hir/hir.h>
#include <hir/cfg.h>

typedef struct {
    cfg_block_t* header;
    cfg_block_t* latch;
    set_t        blocks;
    list_t       children;
} loop_node_t;

typedef struct {
    list_t loops;
} ltree_ctx_t;

/*
[Analyzation] Basic search function for cycles in the provided CFG.
Note: Will change the cfg_block_t's type to CFG_LOOP_HEADER or CFG_LOOP_LATCH.
Note 2: This function will produce a loop tree.
Params:
    - `cctx` - CFG context.

Returns 1 if success, otherwise 0.
*/
int HIR_LOOP_mark_loops(cfg_ctx_t* cctx);

/*
[Transformation] This function will search for loops, then trasform them to canonical form.
Note: Canonical form implies pre-header existance. See REAME for additional info.
Params:
    - `cctx` - CFG context.

Returns 1 if success, otherwise 0.
*/
int HIR_LTREE_canonicalization(cfg_ctx_t* cctx);
int HIR_LTREE_licm(cfg_ctx_t* cctx, sym_table_t* smt);
int HIR_LTREE_build_loop_tree(cfg_func_t* fb, ltree_ctx_t* ctx);
int HIR_LTREE_unload_ctx(ltree_ctx_t* ctx);

#endif