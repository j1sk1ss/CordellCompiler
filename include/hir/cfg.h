#ifndef CFG_H_
#define CFG_H_

#include <std/mm.h>
#include <std/set.h>
#include <std/str.h>
#include <std/list.h>
#include <hir/hir.h>
#include <lir/lir.h>

typedef struct {
    /* Basic info and content */
    long         id;
    hir_block_t* entry;
    hir_block_t* exit;

    /* CFG data */
    set_t        leaders;     /* Leaders for block generation     */
    set_t        terminators; /* Terminators for block generation */
    list_t       blocks;
} cfg_func_t;

typedef struct {
    hir_block_t* entry;
    hir_block_t* exit;
} hir_map_t;

typedef struct {
    lir_block_t* entry;
    lir_block_t* exit;
} lir_map_t;

typedef struct cfg_block {
    /* Basic info and content */
    long              id;
    cfg_func_t*       pfunc; /* parent function */

    hir_map_t         hmap; /* Mapping to exister HIR ctx */
    lir_map_t         lmap; /* Mapping to existed LIR ctx */

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
    long   cid;
    list_t funcs;
} cfg_ctx_t;

int HIR_CFG_cleanup_blocks_temporaries(cfg_ctx_t* cctx);

int HIR_CFG_compute_domf(cfg_func_t* func);
int HIR_CFG_compute_dom(cfg_func_t* func);
int HIR_CFG_compute_sdom(cfg_func_t* func);
int HIR_CFG_collect_defs_by_id(long v_id, cfg_ctx_t* cctx, set_t* out);
int HIR_CFG_create_domdata(cfg_ctx_t* cctx);

int HIR_CFG_split_by_functions(hir_ctx_t* hctx, cfg_ctx_t* ctx);
cfg_block_t* HIR_CFG_function_findlb(cfg_func_t* f, long lbid);

cfg_block_t* CFG_create_cfg_block(hir_block_t* e);
int CFG_insert_cfg_block(cfg_func_t* f, cfg_block_t* b, cfg_block_t* next);
int HIR_CFG_mark_leaders(cfg_ctx_t* ctx);
int HIR_CFG_make_allias(cfg_ctx_t* cctx, sym_table_t* smt);
int HIR_CFG_build(hir_ctx_t* hctx, cfg_ctx_t* ctx);
int HIR_CFG_unload(cfg_ctx_t* ctx);

#endif