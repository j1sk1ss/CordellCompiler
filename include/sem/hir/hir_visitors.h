#ifndef HIR_VISITORS_H_
#define HIR_VISITORS_H_

#include <std/mem.h>
#include <std/queue.h>
#include <std/tuple.h>
#include <sem/misc/trace.h>
#include <sem/misc/warns.h>
#include <sem/hir/hir_data.h>
#include <sem/hir/syscall.h>
#include <sem/hir/z3_wrapper.h>
#include <prep/token_types.h>
#include <ast/ast.h>
#include <hir/hir.h>
#include <hir/hirgen.h>
#include <hir/hir_types.h>
#include <hir/cfg.h>
#include <hir/dag.h>

/* Check whether a dereferenced subject can be equal to NULL and report an error
trace when it can.
Params:
    - `hb` - HIR block where dereference is performed.
    - `s` - Dereferenced subject.
    - `f` - Function virtual form for Z3 checks.
    - `smt` - Symtable.
    - `ctx` - HIR visitors context.

Returns 1 if the check succeeds, otherwise 0 */
int HIR_SEM_check_subject_value_and_provide_trace(
    hir_block_t* hb, cfg_block_t* bb, hir_subject_t* s, sym_table_t* smt, hir_visitors_ctx_t* ctx,
    long long value, char* error
);

int HIRWLKR_visit_setpos_instruction(HIR_VISITOR_ARGS);
int HIRWLKR_visit_phi_instruction(HIR_VISITOR_ARGS);
int HIRWLKR_visit_gdref_instruction(HIR_VISITOR_ARGS);
int HIRWLKR_visit_ldref_instruction(HIR_VISITOR_ARGS);
int HIRWLKR_visit_ifop2_instruction(HIR_VISITOR_ARGS);
int HIRWLKR_wrong_arg_type(HIR_VISITOR_ARGS);
int HIRWLKR_visit_syscall_instruction(HIR_VISITOR_ARGS);
int HIRWLKR_unused_rtype(HIR_VISITOR_ARGS);
int HIRWLKR_noret_assign(HIR_VISITOR_ARGS);
int HIRWLKR_ref_to_expression(HIR_VISITOR_ARGS);
int HIRWLKR_null_free(HIR_VISITOR_ARGS);
int HIRWLKR_zero_malloc(HIR_VISITOR_ARGS);

#endif
