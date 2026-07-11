#ifndef CFG_H_
#define CFG_H_

#include <std/mm.h>
#include <std/set.h>
#include <std/str.h>
#include <std/list.h>
#include <std/stack.h>
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
    symbol_id_t  f_id;
    hir_map_t    hmap;    /* Mapping to exister HIR ctx   */
    lir_map_t    lmap;    /* Mapping to existed LIR ctx   */

    /* CFG data */
    set_t        locals;  /* Local functions              */
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
    cfg_block_type_t   type;
    long               id;
    cfg_func_t*        pfunc;    /* parent function            */

    hir_map_t          hmap;     /* Mapping to existed HIR ctx */
    lir_map_t          lmap;     /* Mapping to existed LIR ctx */

    /* Block navigation */
    struct cfg_block*  l;
    struct cfg_block*  jmp;
    set_t              pred;     /* cfg_block_t* set            */
    set_t              visitors; /* long id set                 */
    unsigned long long visited;
    
    /* Dominance frontier analysis */
    set_t              dom;      /* Dominators                  */
    struct cfg_block*  sdom;     /* Strict dominators           */
    struct cfg_block*  dom_c;    /* Dominator children          */
    struct cfg_block*  dom_s;    /* Dominator sibling           */
    set_t              domf;     /* Dominance frontier          */

    /* SSA */
    set_t              phi;

    /* Liveness analysis */
    set_t              def;      /* Set of defined variables    */
    set_t              use;      /* Set of used variables       */
    set_t              curr_in;  /* Current IN{} set            */
    set_t              curr_out; /* Current OUT{} set           */
    set_t              prev_in;  /* Prev IN{} set               */
    set_t              prev_out; /* Prev IN{} set               */

    /* Copy propagation */
    set_t              copy_gen;  /* Generated copy targets      */
    set_t              copy_kill; /* Killed copy targets         */
} cfg_block_t;

typedef enum {
    CFG_DFS_CONTINUE,
    CFG_DFS_SKIP,
    CFG_DFS_STOP,
} cfg_dfs_action_t;

typedef struct {
    cfg_block_t* block;
    long         pred;
} cfg_dfs_frame_t;

static inline int __cfg_dfs_push(sstack_t* stack, cfg_block_t* block, long pred) {
    if (!block) return 1;

    cfg_dfs_frame_t* frame = (cfg_dfs_frame_t*)mm_malloc(sizeof(cfg_dfs_frame_t));
    if (!frame) return 0;

    frame->block = block;
    frame->pred  = pred;

    if (!stack_push(stack, frame)) {
        mm_free(frame);
        return 0;
    }

    return 1;
}

/* Walk CFG blocks in depth-first order starting from the `head` block.
Params:
    - `head` - Head CFG block.
    - `start_pred` - Initial predecessor id.
    - `counter` - Shared traversal marker.
    - `logic` - Function-like object with the following signature:
                cfg_dfs_action_t logic(cfg_block_t* bb, long pred, ...).
    - `...` - Optional context arguments passed to `logic`.

Logic returns:
    - `CFG_DFS_CONTINUE` - visit children.
    - `CFG_DFS_SKIP` - stop the current path as successful.
    - `CFG_DFS_STOP` - abort traversal and return 0.

Returns 1 if traversal finished without CFG_DFS_STOP, otherwise 0 */
#define CFG_DFS_WALK_COUNTER(head, start_pred, counter, logic, ...) ({                  \
    int __cfg_dfs_result = 1;                                                           \
    unsigned long long __cfg_dfs_active_counter = (counter);                            \
    sstack_t __cfg_dfs_stack;                                                           \
    if (!stack_init(&__cfg_dfs_stack)) {                                                \
        __cfg_dfs_result = 0;                                                           \
    }                                                                                   \
    else {                                                                              \
        if (!__cfg_dfs_push(&__cfg_dfs_stack, (head), (start_pred))) {                    \
            __cfg_dfs_result = 0;                                                       \
        }                                                                               \
        void* __cfg_dfs_raw = NULL;                                                     \
        while (__cfg_dfs_result && stack_pop(&__cfg_dfs_stack, &__cfg_dfs_raw)) {       \
            cfg_dfs_frame_t* __cfg_dfs_frame = (cfg_dfs_frame_t*)__cfg_dfs_raw;         \
            cfg_block_t* __cfg_dfs_bb = __cfg_dfs_frame->block;                         \
            long __cfg_dfs_pred = __cfg_dfs_frame->pred;                                \
            mm_free(__cfg_dfs_frame);                                                   \
            if (!__cfg_dfs_bb) continue;                                                \
            if (__cfg_dfs_bb->visited != __cfg_dfs_active_counter) {                    \
                set_free(&__cfg_dfs_bb->visitors);                                      \
                set_init(&__cfg_dfs_bb->visitors, SET_NO_CMP);                          \
            }                                                                           \
            if (set_has(&__cfg_dfs_bb->visitors, (void*)__cfg_dfs_pred)) continue;      \
            __cfg_dfs_bb->visited = __cfg_dfs_active_counter;                           \
            set_add(&__cfg_dfs_bb->visitors, (void*)__cfg_dfs_pred);                    \
            cfg_dfs_action_t __cfg_dfs_action = logic(                                  \
                __cfg_dfs_bb,                                                           \
                __cfg_dfs_pred,                                                         \
                ##__VA_ARGS__                                                           \
            );                                                                          \
            if (__cfg_dfs_action == CFG_DFS_STOP) {                                     \
                __cfg_dfs_result = 0;                                                   \
                continue;                                                               \
            }                                                                           \
            if (__cfg_dfs_action == CFG_DFS_SKIP) continue;                             \
            if (                                                                        \
                !__cfg_dfs_push(&__cfg_dfs_stack, __cfg_dfs_bb->jmp, __cfg_dfs_bb->id) || \
                !__cfg_dfs_push(&__cfg_dfs_stack, __cfg_dfs_bb->l, __cfg_dfs_bb->id)      \
            ) __cfg_dfs_result = 0;                                                     \
        }                                                                               \
        stack_free_force(&__cfg_dfs_stack);                                             \
    }                                                                                   \
    __cfg_dfs_result;                                                                   \
})

#define CFG_DFS_WALK(head, logic, ...)                                                  \
    CFG_DFS_WALK_COUNTER((head), -1, CFG_get_unique_counter(), logic, ##__VA_ARGS__)

#define iterate_hir_instructions(bb) \
    for (hir_block_t* hh = HIR_get_next(bb->hmap.entry, bb->hmap.exit, 0); hh; hh = HIR_get_next(hh, bb->hmap.exit, 1))
#define iterate_lir_instructions(bb) \
    for (lir_block_t* lh = LIR_get_next(bb->lmap.entry, bb->lmap.exit, 0); lh; lh = LIR_get_next(lh, bb->lmap.exit, 1))

typedef struct {
    long       cid;   /* Current block number: Service info */
    list_t     funcs; /* Function blocks                    */
    map_t      fmap;  /* Function map id<->function_block   */
    struct {
        list_t hout;  /* HIR blocks out from a function     */
        list_t lout;
    } outs;
} cfg_ctx_t;

/*
Return a process-wide unique counter for CFG traversals.
The zero value is reserved for blocks which haven't been visited.
*/
unsigned long long CFG_get_unique_counter();

/*
Perform cleanup operation for CFG blocks. Will clean all information related to navigation.
Note: Will free and re-init all sets related to visitors, etc.
Params:
    - `cctx` - CFG.

Returns 1 on success, otherwise 0.
*/
int HIR_CFG_cleanup_navigation(cfg_ctx_t* cctx);

/*
Compute dominance frontier based on dominance data.
Params:
    - `func` - CFG func.

Returns 1 on success, otherwise 0.
*/
int HIR_CFG_compute_domf(cfg_func_t* func);

/*
Compute dominance based on function CFG.
Params:
    - `func` - CFG func.

Returns 1 on success, otherwise 0.
*/
int HIR_CFG_compute_dom(cfg_func_t* func);

/*
Compute strict dominance based on CFG dominance.
Params:
    - `func` - CFG func.

Returns 1 on success, otherwise 0.
*/
int HIR_CFG_compute_sdom(cfg_func_t* func);

/*
Compute all dominance-related data (dominance, strict dominance, dominance frontier).
Params:
    - `func` - CFG func.

Returns 1 on success, otherwise 0.
*/
int HIR_CFG_create_domdata(cfg_ctx_t* cctx);

/*
Compute all dominance-related data (dominance, strict dominance, dominance frontier).
Params:
    - `func` - CFG func.

Returns 1 on success, otherwise 0.
*/
int HIR_CFG_unload_domdata(cfg_ctx_t* cctx);

/*
Split input HIR sequence and generate CFG functions.
Params:
    - `hctx` - HIR.
    - `ctx` - CFG.
    - `smt` - Symtable.

Returns 1 on success, otherwise 0.
*/
int HIR_CFG_split_by_functions(hir_ctx_t* hctx, cfg_ctx_t* ctx, sym_table_t* smt);

/*
Find base block that starts with provided label ID.
Params:
    - `f` - Function CFG.
    - `lbid` - Label ID.

Return NULL if not found, or base block.
*/
cfg_block_t* HIR_CFG_function_findlb(cfg_func_t* f, unsigned long lbid);

/*
Remove HIR block to CFG. Will change entry and exit links, if they are not set yet.
Params:
    - `bb` - Base block.
    - `hh` - HIR block.

Returns 1 on success, otherwise 0.
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
Complete leaders list in function CFG.
Params:
    - `ctx` - CFG.

Returns 1 on success, otherwise 0.
*/
int HIR_CFG_mark_leaders(cfg_ctx_t* ctx);

/*
Create allias information (owning information).
Params:
    - `cctx` - CFG.
    - `smt` - Symtable.

Returns 1 on success, otherwise 0.
*/
int HIR_CFG_make_allias(cfg_ctx_t* cctx, sym_table_t* smt);

/*
Build CFG from HIR.
Params:
    - `hctx` - HIR.
    - `ctx` - CFG ctx.
    - `smt` - Symtable.

Returns 1 on success, otherwise 0.
*/
int HIR_CFG_build(hir_ctx_t* hctx, cfg_ctx_t* ctx, sym_table_t* smt);

/*
Get all blocks which don't have predcessors, then check whether they
are an entry points or not. If they aren't - remove them and clear
its HIR blocks.
Note 1: It's an important action before dominance calculation, given the
CFG generation artifacts!
Note 2: Perform this operation !ONLY! before the !LAST! dominance calculation. 
Params:
    - `ctx` - CFG context.

Returns 1 if succeeds. 
*/
int HIR_CFG_finilize_before_dom(cfg_ctx_t* ctx);

/*
Get the count of HIR blocks in base block.
Params:
    - `bb` - Basic block.
    - `debug` - Include debug and other non-functional blocks?

Returns the count of HIR blocks in the BB.
*/
int HIR_CFG_count_blocks_in_bb(cfg_block_t* bb, int debug);

/*
Unite close blocks to one block. Idea is to get rid from
a ton of small blocks that are being connected with each 
other via one link.
To find such blocks, we unite all block which don't have
two links.
Params:
    - `ctx` - CFG context.

Returns 1 if succeeds.
*/
int HIR_CFG_squeeze_blocks(cfg_ctx_t* ctx);

/*
Unload CFG.
Params:
    - `ctx` - CFG.

Returns 1 on success, otherwise 0.
*/
int HIR_CFG_unload(cfg_ctx_t* ctx);

/*
Get next instruction from a function block.
Params:
    - `curr` - Current instruction.
    - `fb` - Function block.
    - `opt_exit` - Optional exit block. By default the fb exit is used.
    - `skip` - How many we need to skip? (0 means return the start inst from fb).

Returns next instruction or NULL is there is no instructions. 
*/
hir_block_t* HIR_FUNC_get_next(hir_block_t* curr, cfg_func_t* fb, hir_block_t* opt_exit, int skip);

#endif
