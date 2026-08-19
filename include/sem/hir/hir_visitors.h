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

typedef enum {
    HIR_VALUE_TRACE_POSSIBLE,
    HIR_VALUE_TRACE_EXACT
} hir_value_trace_mode_t;

/* Check whether a subject can be equal to a value and report an error trace
when it can.
Params:
    - `hb` - HIR block where the checked operation is performed.
    - `s` - Checked subject.
    - `smt` - Symtable.
    - `ctx` - HIR visitors context.
    - `value` - Value to compare with.
    - `value_name` - User-facing value name. Pass NULL to use `value`.
    - `mode` - Whether possible equality is enough, or exact equality is required.

Returns 1 if the check succeeds, otherwise 0 */
int HIR_SEM_check_subject_value_and_provide_trace_ex(
    hir_block_t* hb, cfg_block_t* bb, hir_subject_t* s, sym_table_t* smt, hir_visitors_ctx_t* ctx,
    long long value, const char* value_name, hir_value_trace_mode_t mode, const char* error
);

int HIR_SEM_check_subject_value_and_provide_trace(
    hir_block_t* hb, cfg_block_t* bb, hir_subject_t* s, sym_table_t* smt, hir_visitors_ctx_t* ctx,
    long long value, const char* error
);

/* general */
int HIRWLKR_visit_setpos_instruction(HIR_VISITOR_ARGS);
int HIRWLKR_visit_phi_instruction(HIR_VISITOR_ARGS);
int HIRWLKR_null_notnull(HIR_VISITOR_ARGS);
int HIRWLKR_visit_gdref_instruction(HIR_VISITOR_ARGS);
int HIRWLKR_visit_ldref_instruction(HIR_VISITOR_ARGS);
int HIRWLKR_visit_ifop2_instruction(HIR_VISITOR_ARGS);
int HIRWLKR_wrong_arg_type(HIR_VISITOR_ARGS);
int HIRWLKR_visit_syscall_instruction(HIR_VISITOR_ARGS);
int HIRWLKR_unused_rtype(HIR_VISITOR_ARGS);
int HIRWLKR_noret_assign(HIR_VISITOR_ARGS);
int HIRWLKR_ref_to_expression(HIR_VISITOR_ARGS);
int HIRWLKR_division_by_zero(HIR_VISITOR_ARGS);
int HIRWLKR_division_by_one(HIR_VISITOR_ARGS);
int HIRWLKR_bad_buffer_move(HIR_VISITOR_ARGS);
int HIRWLKR_illegal_store(HIR_VISITOR_ARGS);

/* glibc */
int HIRWLKR_glibc_arg_checkers(HIR_VISITOR_ARGS);
int HIRWLRK_glibc_double_free_checker(HIR_VISITOR_ARGS);

#endif
