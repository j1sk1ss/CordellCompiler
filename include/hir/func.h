#ifndef FUNC_H_
#define FUNC_H_

#include <std/mm.h>
#include <std/set.h>
#include <std/str.h>
#include <hir/hir.h>
#include <hir/cfg.h>
#include <symtab/functb.h>

typedef struct {
    char  flag;
    long  fid;
    set_t edges;
} call_graph_node_t;

typedef struct {
    long  e_fid;
    map_t verts;
} call_graph_t;

/*
[Transformation] Apply call graph information to CFG.
Note: Will mark functions as unused if they aren't connected to each other
in call graph.
Params:
    - `cctx` - CFG context.
    - `ctx` - Call graph context.

Returns 1 if operation succeed. Otherwise it will return 0.
*/
int HIR_CG_apply_dfe(cfg_ctx_t* cctx, call_graph_t* ctx);

/*
[Analyzation] Perform basic call graph generation.
Note: Will connect call graph's functions with each other.
Params:
    - `ctx` - Call graph itself.
    - `smt` - Symtable.

Returns 1 if operation succeed. Otherwise it will return 0.
*/
int HIR_CG_perform_dfe(call_graph_t* ctx, sym_table_t* smt);

/*
Build basic call graph based on the provided CFG.
Params:
    - `cctx` - CFG context.
    - `ctx` - Call graph context.
    - `smt` - Symtable.

Returns 1 if operation succeed. Otherwise it will return 0.
*/
int HIR_CG_build(cfg_ctx_t* cctx, call_graph_t* ctx, sym_table_t* smt);

/*
Unload call graph.
Params:
    - `ctx` - Call graph.

Returns 1 if operation succeed. Otherwise it will return 0.
*/
int HIR_CG_unload(call_graph_t* ctx);

/*
Perform tail-recursion elimination. Will remove all tail recursions with cycle.
Params:
    - `cctx` - CFG.
    - `smt` - Symtable.

Return 1 if success, otherwise 0.
*/
int HIR_FUNC_perform_tre(cfg_ctx_t* cctx, sym_table_t* smt);

/*
Perform inlining optimization. Will inline function that get 3 euristic score points.
Points earned with next logic:
- inlinng in loop: +2
- <= 2 base blocks in function: +3
- <= 5 base blocks in function: +2
- <= 10 base blocks in function: +1
- > 15 base blocks in function: -2

Params:
    - `cctx` - CFG.

Return 1 if success, otherwise 0.
*/
int HIR_FUNC_perform_inline(cfg_ctx_t* cctx);

#endif