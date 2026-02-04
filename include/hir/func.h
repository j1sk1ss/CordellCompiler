#ifndef FUNC_H_
#define FUNC_H_

#include <std/mm.h>
#include <std/set.h>
#include <std/str.h>
#include <symtab/functb.h>
#include <ast/ast.h>
#include <hir/hir.h>
#include <hir/cfg.h>
#include <hir/loop.h>

typedef struct {
    char  flag : 1;
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
Note 2: Will mark functions as an unused function!
        Must be called after all possible HIR optimizations!
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
Params:
    - `cctx` - CFG.
    - `smt` - Symtable.
    - `checker` - Function desider.

Return 1 if success, otherwise 0.
*/
int HIR_FUNC_perform_inline(cfg_ctx_t* cctx, sym_table_t* smt, int (*checker)(int*, int));

/*
Euristic inline desider (basic option).
Params:
    - `data` - Desider data.
    - `size` - Desider data size.

Returns 1 if function must be inlined.
*/
int HIR_FUNC_inline_euristic_desider(int* data, int size);

/*
De-virtualization for functions in HIR.
The idea to determine which function is beign called:
```cpl
function foo(i32 a) => i0;  : id=0 :
function foo(i32 a) => i32; : id=1 :
function foo(i8 a) => 0;    : id=2 :

{
    foo(10 as i32);         : id=0 :
    i32 a = foo(10 as i32); : id=1 :
    foo(1 as i8);           : id=2 :
}
```

Note: De-virtualization works only with determined arguments. To determine arguments,
the user must use the 'as' keyword for convertion.

The function will change the call function id based on the provided arguemnts and the return type.
Params:
    - `cctx` - CFG context.
    - `smt` - Symtable.

Returns 1 if succeeds.
*/
int HIR_FUNC_perform_devirt(cfg_ctx_t* cctx, sym_table_t* smt);

#endif