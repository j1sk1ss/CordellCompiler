#ifndef X86_64_IRGEN_H_
#define X86_64_IRGEN_H_

#include <hir/hir.h>
#include <hir/hir_types.h>
#include <lir/lir.h>
#include <lir/lirgen.h>
#include <lir/lir_types.h>
#include <symtab/symtab.h>
#include <std/map.h>
#include <std/qsort.h>
#include <std/stack.h>
#include <std/stackmap.h>

/*
Generate the LIR preambule for a function.
Params:
    - `op` - Call operation.
    - `ctx` - LIT context.
    - `hir_args` - Source HIR call agruments.
    - `lir_args` - Output LIR call arguments.

Returns 1 if succeeds.
*/
int x86_64_pass_params(lir_operation_t op, lir_ctx_t* ctx, list_t* hir_args, list_t* lir_args);

/*
Convert the HIR variable to a LIR variable.
Params:
    - `subj` - HIR subject to convert.

Returns a LIR variable. Otherwise will return 0.
*/
lir_subject_t* x86_64_format_variable(hir_subject_t* subj);

/*
Generate a LIR store operation from the 'src' to the 'dst'.
Params:
    - `op` - Store operation.
    - `ctx` - LIR context.
    - `dst` - Destination subject.
    - `src` - Source subject.

Returns 1 if succeeds.
*/
int x86_64_store_var2var(lir_operation_t op, lir_ctx_t* ctx, hir_subject_t* dst, hir_subject_t* src);

/*
Generate a LIR code from a HIR code.
Params:
    - `cctx` - CFG context.
    - `ctx` - LIR context.
    - `smt` - Symtable.

Returns 1 if succeeds.
*/
int LIR_generate_block(cfg_ctx_t* cctx, lir_ctx_t* ctx, sym_table_t* smt);

#endif