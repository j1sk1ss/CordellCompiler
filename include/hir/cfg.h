#ifndef CFG_H_
#define CFG_H_

#include <std/mm.h>
#include <std/set.h>
#include <std/str.h>
#include <std/list.h>
#include <symtab/symtab.h>
#include <hir/hir.h>
#include <hir/hirgen.h>
#include <lir/lir.h>

typedef struct {
    hir_block_t* entry;
    hir_block_t* exit;
} hir_map_t;

typedef struct {
    lir_block_t* entry;
    lir_block_t* exit;
} lir_map_t;

typedef struct {
    /* Meta flags */
    char         used   : 1;
    char         fentry : 1;

    /* Basic info and content */
    long         id;
    long         fid;
    hir_map_t    hmap;     /* Mapping to exister HIR ctx */
    lir_map_t    lmap;     /* Mapping to existed LIR ctx */

    /* CFG data */
    set_t        leaders; /* Leaders for block generation */
    list_t       blocks;  /* cfg_block_t* list            */
} cfg_func_t;

typedef enum {
    CFG_DEFAULT_BLOCK,
    CFG_LOOP_PREHEADER,
    CFG_LOOP_HEADER,
    CFG_LOOP_BLOCK,
    CFG_LOOP_LATCH,
} cfg_block_type_t;

typedef struct cfg_block {
    /* Basic info and content */
    cfg_block_type_t  type;
    long              id;
    cfg_func_t*       pfunc;    /* parent function            */

    hir_map_t         hmap;     /* Mapping to existed HIR ctx */
    lir_map_t         lmap;     /* Mapping to existed LIR ctx */

    /* Block navigation */
    struct cfg_block* l;
    struct cfg_block* jmp;
    set_t             pred;     /* cfg_block_t* set            */
    set_t             visitors; /* long id set                 */
    unsigned int      visited;
    
    /* Dominance frontier analysis */
    set_t             dom;      /* Dominators                  */
    struct cfg_block* sdom;     /* Strict dominators           */
    struct cfg_block* dom_c;    /* Dominator children          */
    struct cfg_block* dom_s;    /* Dominator sibling           */
    set_t             domf;     /* Dominance frontier          */

    /* SSA */
    set_t             phi;

    /* Liveness analysis */
    set_t             def;      /* Set of defined variables    */
    set_t             use;      /* Set of used variables       */
    set_t             curr_in;  /* Current IN{} set            */
    set_t             curr_out; /* Current OUT{} set           */
    set_t             prev_in;  /* Prev IN{} set               */
    set_t             prev_out; /* Prev IN{} set               */
} cfg_block_t;

typedef struct {
    long   cid;
    list_t funcs; /* Function blocks                */
    list_t out;   /* HIR blocks out from a function */
} cfg_ctx_t;

/*
Perform cleanup operation for CFG blocks. Will clean all information related to navigation.
Note: Will free and re-init all sets related to visitors, etc.
Params:
    - `cctx` - CFG.

Return 1 if success, otherwise 0.
*/
int HIR_CFG_cleanup_navigation(cfg_ctx_t* cctx);

/*
Perform cleanup operation for CFG block. Will clean all tmp information, like leaders, prev_in/out.
Params:
    - `cctx` - CFG.

Return 1 if success, otherwise 0.
*/
int HIR_CFG_cleanup_blocks_temporaries(cfg_ctx_t* cctx);

/*
Compute dominance frontier based on dominance data.
Params:
    - `func` - CFG func.

Return 1 if success, otherwise 0.
*/
int HIR_CFG_compute_domf(cfg_func_t* func);

/*
Compute dominance based on function CFG.
Params:
    - `func` - CFG func.

Return 1 if success, otherwise 0.
*/
int HIR_CFG_compute_dom(cfg_func_t* func);

/*
Compute strict dominance based on CFG dominance.
Params:
    - `func` - CFG func.

Return 1 if success, otherwise 0.
*/
int HIR_CFG_compute_sdom(cfg_func_t* func);

/*
Compute all dominance-related data (dominance, strict dominance, dominance frontier).
Params:
    - `func` - CFG func.

Return 1 if success, otherwise 0.
*/
int HIR_CFG_create_domdata(cfg_ctx_t* cctx);

/*
Compute all dominance-related data (dominance, strict dominance, dominance frontier).
Params:
    - `func` - CFG func.

Return 1 if success, otherwise 0.
*/
int HIR_CFG_unload_domdata(cfg_ctx_t* cctx);

/*
Split input HIR sequence and generate CFG functions.
Params:
    - `hctx` - HIR.
    - `ctx` - CFG.
    - `smt` - Symtable.

Return 1 if success, otherwise 0.
*/
int HIR_CFG_split_by_functions(hir_ctx_t* hctx, cfg_ctx_t* ctx, sym_table_t* smt);

/*
Find base block that starts with provided label ID.
Params:
    - `f` - Function CFG.
    - `lbid` - Label ID.

Return NULL if not found, or base block.
*/
cfg_block_t* HIR_CFG_function_findlb(cfg_func_t* f, long lbid);

/*
Append HIR block to CFG. Will change entry and exit links, if they are not set yet.
Params:
    - `bb` - Base block.
    - `hh` - HIR block.

Return 1 if success, otherwise 0.
*/
int HIR_CFG_append_hir_block_back(cfg_block_t* bb, hir_block_t* hh);

/*
Remove HIR block to CFG. Will change entry and exit links, if they are not set yet.
Params:
    - `bb` - Base block.
    - `hh` - HIR block.

Return 1 if success, otherwise 0.
*/
int HIR_CFG_remove_hir_block(cfg_block_t* bb, hir_block_t* hh);

/*
Create CFG base block.
Params:
    - `e` - Entry HIR block.

Return cfg base block or NULL if something goes wrong.
*/
cfg_block_t* HIR_CFG_create_cfg_block(hir_block_t* e);

/*
Insert CFG base block into function list of blocks.
Params:
    - `f` - Function CFG.
    - `b` - Base block.
    - `next` - Next base block in function list.

Return 1 if success, otherwise 0.
*/
int HIR_CFG_insert_cfg_block_before(cfg_func_t* f, cfg_block_t* b, cfg_block_t* next);

/*
Complete leaders list in function CFG.
Params:
    - `ctx` - CFG.

Return 1 if success, otherwise 0.
*/
int HIR_CFG_mark_leaders(cfg_ctx_t* ctx);

/*
Create allias information (owning information).
Params:
    - `cctx` - CFG.
    - `smt` - Symtable.

Return 1 if success, otherwise 0.
*/
int HIR_CFG_make_allias(cfg_ctx_t* cctx, sym_table_t* smt);

/*
Build CFG from HIR.
Params:
    - `hctx` - HIR.
    - `ctx` - CFG ctx.
    - `smt` - Symtable.

Return 1 if success, otherwise 0.
*/
int HIR_CFG_build(hir_ctx_t* hctx, cfg_ctx_t* ctx, sym_table_t* smt);

/*
Get the count of HIR blocks in base block.
Params:
    - `bb` - Basic block.

Returns the count of HIR blocks in the BB.
*/
int HIR_CFG_count_blocks_in_bb(cfg_block_t* bb);

/*
Unload CFG.
Params:
    - `ctx` - CFG.

Return 1 if success, otherwise 0.
*/
int HIR_CFG_unload(cfg_ctx_t* ctx);

#endif