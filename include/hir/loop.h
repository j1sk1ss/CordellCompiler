#ifndef LOOP_H_
#define LOOP_H_

#include <std/mm.h>
#include <std/set.h>
#include <std/map.h>
#include <std/logg.h>
#include <std/list.h>
#include <hir/hir.h>
#include <hir/cfg.h>

typedef struct loop_node {
    cfg_block_t*      header;   /* Loop header         */
    cfg_block_t*      latch;    /* Loop latch          */
    set_t             blocks;   /* cfg_block_t* blocks */
    list_t            children; /* loop_node_t* nested */
    struct loop_node* p;
} loop_node_t;

typedef struct {
    map_t lmap; /* f.id <-> list_t of loops */
} ltree_ctx_t;

/*
[Analyzation] Basic search function for cycles in the provided CFG.
Note: Will change the cfg_block_t's type to CFG_LOOP_HEADER or CFG_LOOP_LATCH.
Note 2: This function will produce a loop tree.
Params:
    - `cctx` - CFG context.
    - `lctx` - Loops context.

Returns 1 if success, otherwise 0.
*/
int HIR_LOOP_mark_loops(cfg_ctx_t* cctx, ltree_ctx_t* lctx);

/*
[Transformation] This function will search for loops, then trasform them to canonical form.
Note: Canonical form implies pre-header existance. See REAME for additional info.
Params:
    - `cctx` - CFG context.
    - `lctx` - Loops context.

Returns 1 if success, otherwise 0.
*/
int HIR_LTREE_canonicalization(cfg_ctx_t* cctx, ltree_ctx_t* lctx);

/*
[Transformation] Perform LICM optimization on the canonicolized loops.
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

Returns 1 if succeeds.
*/
int HIR_LTREE_licm(cfg_ctx_t* cctx, ltree_ctx_t* lctx, sym_table_t* smt);

/*
[Analyzation] Get the size of the nested loop. It means, it will
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

Returns the size of nested structure.
*/
int HIR_LTREE_nested_count(loop_node_t* node);

// TODO: docs
int HIR_LOOP_perform_dle(ltree_ctx_t* lctx);

/*
Unload the loop context.
Params:
    - `ctx` - Loop context.

Returns 1 if succeeds.
*/
int HIR_LTREE_unload_ctx(ltree_ctx_t* ctx);

#endif