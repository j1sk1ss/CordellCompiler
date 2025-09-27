#ifndef CFG_H_
#define CFG_H_

#include <std/mm.h>
#include <std/set.h>
#include <std/str.h>
#include <std/list.h>
#include <hir/hir.h>

typedef struct cfg_block {
    /* Basic info and content */
    long              id;
    hir_block_t*      entry;
    hir_block_t*      exit;

    /* Block navigation */
    struct cfg_block* l;
    struct cfg_block* jmp;
    set_t             pred;
    set_t             visitors;
    
    /* Dominance frontier analysis */
    set_t             dom;      /* Dominators               */
    struct cfg_block* sdom;     /* Strict dominators        */
    struct cfg_block* dom_c;    /* Dominator children       */
    struct cfg_block* dom_s;    /* Dominator sibling        */
    set_t             domf;     /* Dominance frontier       */

    /* Liveness analysis */
    set_t             def;      /* Set of defined variables */
    set_t             use;      /* Set of used variables    */
    set_t             curr_in;  /* Current IN{} set         */
    set_t             curr_out; /* Current OUT{} set        */
    set_t             prev_in;  /* Prev IN{} set            */
    set_t             prev_out; /* Prev IN{} set            */
} cfg_block_t;

typedef struct {
    hir_block_t* h;
} leader_t;

typedef struct {
    list_t lst;
} leader_ctx_t;

typedef struct {
    /* Basic info and content */
    long         id;
    hir_block_t* entry;
    hir_block_t* exit;

    /* CFG data */
    leader_ctx_t leaders;   /* Leaders for block generation */
    list_t       blocks;
} cfg_func_t;

typedef struct {
    long   cid;
    list_t funcs;
} cfg_ctx_t;

int HIR_CFG_compute_domf(cfg_func_t* func);
int HIR_CFG_compute_dom(cfg_func_t* func);
int HIR_CFG_compute_sdom(cfg_func_t* func);
int HIR_CFG_collect_defs_by_id(long v_id, cfg_ctx_t* cctx, set_t* out);

int HIR_CFG_split_by_functions(hir_ctx_t* hctx, cfg_ctx_t* ctx);
cfg_block_t* HIR_CFG_function_findlb(cfg_func_t* f, long lbid);

int HIR_CFG_mark_leaders(cfg_ctx_t* ctx);
int HIR_CFG_leaders_contains(hir_block_t* h, leader_ctx_t* ctx);
int HIR_CFG_unload_leaders(leader_ctx_t* ctx);

int HIR_build_cfg(hir_ctx_t* hctx, cfg_ctx_t* ctx);

#endif