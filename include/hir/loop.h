#ifndef LOOP_H_
#define LOOP_H_

#include <errno.h>
#include <stdlib.h>

#include <std/mm.h>
#include <std/set.h>
#include <std/map.h>
#include <std/logg.h>
#include <std/list.h>
#include <hir/interval.h>
#include <hir/hir.h>
#include <hir/cfg.h>

typedef struct loop_node {
    cfg_block_t*      header;    /* Loop header                */
    cfg_block_t*      preheader; /* Preheader (Optional)       */
    cfg_block_t*      latch;     /* Loop latch                 */
    set_t             blocks;    /* cfg_block_t* blocks        */
    set_t             exits;
    list_t            blocks_sorted;
    list_t            children;  /* loop_node_t* nested        */
    set_t             ind;       /* Inductive variables (v_id) */
    struct loop_node* p;
} loop_node_t;

typedef struct {
    map_t             lmap; /* f.id <-> list_t of loops */
} ltree_ctx_t;

typedef enum {
    LOOP_ITERATION_UNKNOWN,
    LOOP_ITERATION_FINITE,
    LOOP_ITERATION_INFINITE,
} loop_iteration_status_t;

#define LOOP_ITERATION_COUNT_UNKNOWN  (-2)
#define LOOP_ITERATION_COUNT_INFINITE (-1)

typedef struct {
    hir_subject_t*          iterator;                  /* Current SSA value (PHI result).                 */
    hir_subject_t*          initial;                   /* Initial SSA value, if defined.                  */
    hir_subject_t*          next;                      /* Value on the backedge.                          */
    symbol_id_t             initial_id;
    symbol_id_t             next_id;
    hir_block_t*            phi;
    hir_block_t*            update;                    /* ADD/SUB defining the next value.                */
    cfg_block_t*            update_block;
    hir_subject_t*          step;                      /* Original step operand.                          */
    big_int_t               initial_value;
    big_int_t               step_value;                /* Normalized: iterator += step.                   */
    int                     initial_known;
    int                     step_known;
} loop_iterator_info_t;

typedef struct {
    cfg_block_t*            block;                     /* CFG block containing the test.                  */
    hir_block_t*            branch;                    /* Original exit IF instruction.                   */
    hir_block_t*            comparison;                /* NULL for a bare integer test.                   */
    loop_iterator_info_t*   iterator;                  /* NULL if not recognized.                         */
    hir_subject_t*          bound;                     /* NULL for an implicit zero.                      */
    hir_operation_t         op;                        /* Normalized continuation test.                   */
    big_int_t               bound_value;
    int                     bound_known;
    int                     posttested;
} loop_iteration_condition_t;

typedef struct {
    loop_iteration_status_t status;
    big_int_t               count;                    /* Body executions: >= 0, -1 infinite, -2 unknown. */
    list_t                  iterators;                /* Owned loop_iterator_info_t* records.            */
    list_t                  conditions;               /* Owned loop_iteration_condition_t* records.      */
    set_t                   iteration_dependency_ids; /* Variable IDs needed to derive the count.        */
} loop_iteration_info_t;

/* [Analyzation] Basic search function for cycles in the provided CFG.
   Note: Will change the cfg_block_t's type to CFG_LOOP_HEADER or CFG_LOOP_LATCH.
   Note 2: This function will produce a loop tree.
   Params:
       - `cctx` - CFG context.
       - `lctx` - Loops context.
   
   Returns 1 on success, otherwise 0. */
int HIR_LOOP_mark_loops(cfg_ctx_t* cctx, ltree_ctx_t* lctx);

/* [Transformation] This function will search for loops, then trasform them to canonical form.
   Note: Canonical form implies pre-header existance. See REAME for additional info.
   Params:
       - `cctx` - CFG context.
       - `lctx` - Loops context.
   
   Returns 1 on success, otherwise 0. */
int HIR_LTREE_canonicalization(cfg_ctx_t* cctx, ltree_ctx_t* lctx);

/* Partially unroll eligible non-nested loops by a factor of two before SSA.
   Both copies retain their conditions and updates. Mutates only the HIR sequence.
   The caller must rebuild CFG, dominance and loop tree before further analysis.
   Returns the number of transformed loops, 0 if none qualify, or -1 on failure. */
int HIR_LTREE_unroll(ltree_ctx_t* lctx, sym_table_t* smt);

/* [Transformation] Perform LICM optimization on the canonicolized loops.
   The idea is simple:
       - We have commands that aren't use any loop-depend info
       - These commands are pretty annoying
       - Why we wouldn't move them from a loop?
   This function marks all invariant commands (mentioned not 'loop-dependend' command).
   We move these command to a new created 'preheader' block.
   Params:
       - `cctx` - CFG context.
       - `lctx` - Loops context.
       - `smt` - Symtable.
   
   Returns 1 if succeeds. */
int HIR_LTREE_licm(cfg_ctx_t* cctx, ltree_ctx_t* lctx, sym_table_t* smt);

/* [Analyzation] Get the size of the nested loop. It means, it will
   return the count of all 'parent' loops of the provided loop.
   For instance:
   ```cpl
   loop { : Return 0 :
       loop { : Return 1 :
   
       }
   }
   ```
   
   Params:
       - `node` - The considered loop.
   
   Returns the size of nested structure. */
int HIR_LTREE_nested_count(loop_node_t* node);

/* Mark loop instructions as dead when the loop has no live content.
   We will delete loops which don't have any interaction with outer data,
   and they aren't endless.

    ```cpl
    i32 a = 0i32;
    while a < 10; { :/ We will remove this loop /:
        a += 1;
    }
    ```

    But:

    ```cpl
    while a < 10; { :/ foo preserves the loop from elimination /:
        foo();
    }
    ```

   Params:
       - `lctx` - Loop tree context.
       - `smt` - Symtable populated by constant propagation.
   
   Returns 1 if succeeds. */
int HIR_LOOP_perform_dle(ltree_ctx_t* lctx, sym_table_t* smt);

// TODO: docs
int HIR_LOOP_analyze_iterations(loop_node_t* loop, loop_iteration_info_t* out, sym_table_t* smt);
void HIR_LOOP_free_iteration_info(loop_iteration_info_t* info);

/* Unload the loop context.
   Params:
       - `ctx` - Loop context.
   
   Returns 1 if succeeds. */
int HIR_LTREE_unload_ctx(ltree_ctx_t* ctx);

/* Get CFG blocks with conditional exits from the provided loop.
   If you will provide it with a code:

   ```cpl
   while 1; {
   }
   ```

   Includes tests in the header, latch and other loop blocks.
   The caller initializes and frees the output set.
   Return 1 if succeeds. */
int HIR_LOOP_get_cond_blocks(loop_node_t* loop, set_t* conds);

#endif