/* Main file where proceeds copy propagation. The main idea is to
   propagate values thru registers / variables:
   mov rax, 10
   mov rbx, rax
   mov rcx, rbx
   ->
   mov rcx, 10
*/
#ifndef COPY_PROP_H_
#define COPY_PROP_H_

#include <utils.h>
#include <std/map.h>
#include <std/set.h>
#include <std/list.h>
#include <hir/cfg.h>
#include <lir/lir.h>
#include <lir/lirgen.h>

/* Drop variable writes whose values are never used later.
Params:
    - `cctx` - CFG context.

Returns GENERAL_SUCCESS (1) if succeeds. */
status_code_t LIR_drop_unused_variables(cfg_ctx_t* cctx);

/* Propagate variable copies through LIR instructions.
Params:
    - `cctx` - CFG context.
    - `smt` - Symtable.

Returns GENERAL_SUCCESS (1) if succeeds. */
status_code_t LIR_variable_copy_propagation(cfg_ctx_t* cctx, sym_table_t* smt);

/* Propagate register copies through already register-selected LIR.
Params:
    - `cctx` - CFG context.

Returns GENERAL_SUCCESS (1) if succeeds. */
status_code_t LIR_register_copy_propagation(cfg_ctx_t* cctx);

/* Traverse the LIR context, find unused global variables (by setting
'set' to used variables only)? and set 'unuset' to rest of variables.
Params:
    - `cctx` - CFG context.
    - `smt` - Symbol table.

Returns GENERAL_SUCCESS (1) if succeeds. */
status_code_t LIR_clear_global_variables(cfg_ctx_t* cctx, sym_table_t* smt);

#endif