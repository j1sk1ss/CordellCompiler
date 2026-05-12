#ifndef COPY_PROP_H_
#define COPY_PROP_H_

#include <std/map.h>
#include <std/set.h>
#include <std/list.h>
#include <hir/cfg.h>
#include <lir/lir.h>
#include <lir/lirgen.h>

/*
Drop variable writes whose values are never used later.
Params:
    - `cctx` - CFG context.

Returns 1 if succeeds.
*/
int LIR_drop_unused_variables(cfg_ctx_t* cctx);

/*
Propagate variable copies through LIR instructions.
Params:
    - `cctx` - CFG context.

Returns 1 if succeeds.
*/
int LIR_variable_copy_propagation(cfg_ctx_t* cctx);

/*
Propagate register copies through LIR instructions.
Params:
    - `cctx` - CFG context.

Returns 1 if succeeds.
*/
int LIR_register_copy_propagation(cfg_ctx_t* cctx);

#endif
