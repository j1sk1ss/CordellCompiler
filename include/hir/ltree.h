#ifndef LTREE_H_
#define LTREE_H_

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

int HIR_LTREE_licm_canonicalization(cfg_ctx_t* cctx, sym_table_t* smt);
int HIR_LTREE_build_loop_tree(cfg_func_t* fb, ltree_ctx_t* ctx);
int HIR_LTREE_unload_ctx(ltree_ctx_t* ctx);

#endif