#ifndef CFG_H_
#define CFG_H_

#include <std/set.h>
#include <std/str.h>
#include <std/mm.h>
#include <hir/hir.h>

typedef struct leader {
    hir_block_t*   h;
    struct leader* next;
} leader_t;

typedef struct {
    leader_t* h;
} leader_ctx_t;

typedef struct cfg_block {
    long              id;
    hir_block_t*      entry;
    hir_block_t*      exit;

    struct cfg_block* l;
    struct cfg_block* jmp;
    set_t             pred;
    
    struct cfg_block* next;

    int               visited;

    set_t             dom;
    struct cfg_block* sdom;
    struct cfg_block* dom_c;
    struct cfg_block* dom_s;
    set_t             domf;

    set_t             def;
    set_t             use;
    set_t             curr_in;
    set_t             curr_out;
    set_t             prev_in;
    set_t             prev_out;
} cfg_block_t;

typedef struct cfg_func {
    long             id;
    hir_block_t*     entry;
    hir_block_t*     exit;
    cfg_block_t*     cfg_head;
    leader_ctx_t     leaders;
    struct cfg_func* next;
} cfg_func_t;

typedef struct {
    long        cid;
    cfg_func_t* h;
} cfg_ctx_t;

int HIR_CFG_compute_domf(cfg_func_t* func);
int HIR_CFG_compute_dom(cfg_func_t* func);
int HIR_CFG_compute_sdom(cfg_func_t* func);
int HIR_CFG_collect_defs_by_id(long v_id, cfg_ctx_t* cctx, set_t* out);

int HIR_CFG_split_by_functions(hir_ctx_t* hctx, cfg_ctx_t* ctx);
cfg_func_t* HIR_CFG_find_function(long fid, cfg_ctx_t* ctx);
cfg_block_t* HIR_CFG_function_findlb(cfg_func_t* f, long lbid);

int HIR_CFG_mark_leaders(cfg_ctx_t* ctx);
int HIR_CFG_leaders_contains(hir_block_t* h, leader_ctx_t* ctx);
int HIR_CFG_unload_leaders(leader_ctx_t* ctx);

int HIR_build_cfg(hir_ctx_t* hctx, cfg_ctx_t* ctx);

#endif