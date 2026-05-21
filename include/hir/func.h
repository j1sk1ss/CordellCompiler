#ifndef FUNC_H_
#define FUNC_H_

#include <std/mm.h>
#include <std/set.h>
#include <std/str.h>
#include <std/tuple.h>
#include <std/queue.h>
#include <symtab/functb.h>
#include <ast/ast.h>
#include <hir/hir.h>
#include <hir/cfg.h>
#include <hir/loop.h>
#include <hir/inline_model.h>

typedef struct {
    char  flag;
    long  f_id;
    set_t edges;
} call_graph_node_t;

typedef struct {
    list_t entries;
    map_t  verts;
} call_graph_t;

/*
[Transformation] Apply call graph information to CFG.
Note: Will mark functions as unused if they aren't connected to each other
in call graph.
Params:
    - `cctx` - CFG context.
    - `smt` - Symbol table.

Returns 1 on success, otherwise 0.
*/
int HIR_CG_apply_dfe(cfg_ctx_t* cctx, sym_table_t* smt);

/*
[Analyzation] Perform basic call graph generation.
Note: Will connect call graph's functions with each other.
Note 2: Will mark functions as an unused function!
        Must be called after all possible HIR optimizations!
Params:
    - `ctx` - Call graph itself.
    - `smt` - Symtable.

Returns 1 on success, otherwise 0.
*/
int HIR_CG_perform_dfe(call_graph_t* ctx, sym_table_t* smt);

/*
Build basic call graph based on the provided CFG.
Params:
    - `cctx` - CFG context.
    - `ctx` - Call graph context.
    - `smt` - Symtable.

Returns 1 on success, otherwise 0.
*/
int HIR_CG_build(cfg_ctx_t* cctx, call_graph_t* ctx, sym_table_t* smt);

/*
Unload call graph.
Params:
    - `ctx` - Call graph.

Returns 1 on success, otherwise 0.
*/
int HIR_CG_unload(call_graph_t* ctx);

/*
Perform tail-recursion elimination. Will remove all tail recursions with cycle.
Params:
    - `cctx` - CFG.
    - `smt` - Symtable.

Returns 1 on success, otherwise 0.
*/
int HIR_FUNC_perform_tre(cfg_ctx_t* cctx, sym_table_t* smt);

/*
Perform inlining optimization. Inlines functions that get 3 heuristic score points.
Params:
    - `cctx` - CFG.
    - `lctx` - Loops context.
    - `smt` - Symtable.

Returns 1 on success, otherwise 0.
*/
int HIR_FUNC_perform_inline(cfg_ctx_t* cctx, ltree_ctx_t* lctx, sym_table_t* smt);

/*
Iterate function and find last block. If this is exit and the last block, find HIR_VRUSE, which
signals that there is a return value. Simple example:
```cpl
function foo() {
    i32 a = 1;
    i32 tmp;
    : tmp = : a + 10;
    : use(tmp); :
    return tmp;
}
```
Note: This function will ignore if function doesn't return anything.
Params:
    - `cctx` - CFG context.

Returns 1 if succeeds.
*/
int HIR_FUNC_set_last_return(cfg_ctx_t* cctx);

/*
Delete duplicated functions from the CFG.
Params:
    - `ctx` - CFG context.

Returns 1 on success, otherwise 0.
*/
int HIR_FUNC_delete_duplicated_functions(cfg_ctx_t* ctx);

#endif
