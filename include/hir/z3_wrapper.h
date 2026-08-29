#ifndef SEM_HIR_Z3_WRAPPER_H_
#define SEM_HIR_Z3_WRAPPER_H_

#include <hir/cfg.h>
#include <symtab/symtab_id.h>
#include <symtab/symtab.h>
#include <std/math.h>
#ifdef CPL_ENABLE_Z3
    #include <stdio.h>
    #include <stdlib.h>
    #include <z3.h>
#endif

typedef struct {
#ifdef CPL_ENABLE_Z3
    Z3_config    cfg;
    Z3_context   ctx;
#endif
    cfg_ctx_t*   cfg_ctx;
    sym_table_t* smt;
    map_t        funcs;
#ifdef CPL_ENABLE_Z3
    map_t        callsites;
    map_t        addr_taken;
    int          callsites_ready;
#endif
} z3_analyzer_t;

#define Z3A_NO      0 /* predicate is impossible / always false */
#define Z3A_YES     1 /* predicate is guaranteed / always true */
#define Z3A_MAYBE   2 /* both predicate and its negation are feasible */
#define Z3A_UNKNOWN 3 /* analysis failed, unsupported instruction, unknown var */

typedef int z3_result_t;

z3_analyzer_t* Z3A_create(cfg_ctx_t* cfg, sym_table_t* smt);
int Z3A_unload(z3_analyzer_t* analyzer);

const char* Z3_result_name(int result);

z3_result_t Z3A_is_function_complete(z3_analyzer_t* analyzer, cfg_func_t* function);
z3_result_t Z3_can_vid_be_equal_llong(z3_analyzer_t* analyzer, cfg_func_t* function, symbol_id_t v_id, long long value);
z3_result_t Z3_can_vid_be_not_equal_llong(z3_analyzer_t* analyzer, cfg_func_t* function, symbol_id_t v_id, long long value);
z3_result_t Z3_is_vid_always_equal_llong(z3_analyzer_t* analyzer, cfg_func_t* function, symbol_id_t v_id, long long value);
z3_result_t Z3_is_vid_maybe_equal_llong(z3_analyzer_t* analyzer, cfg_func_t* function, symbol_id_t v_id, long long value);
z3_result_t Z3_is_vid_always_zero(z3_analyzer_t* analyzer, cfg_func_t* function, symbol_id_t v_id);
z3_result_t Z3_is_vid_maybe_zero(z3_analyzer_t* analyzer, cfg_func_t* function, symbol_id_t v_id);
z3_result_t Z3_is_vid_always_nonzero(z3_analyzer_t* analyzer, cfg_func_t* function, symbol_id_t v_id);

z3_result_t Z3_check_subject_eq_llong(z3_analyzer_t* analyzer, cfg_func_t* function, hir_subject_t* subject, long long value);
z3_result_t Z3_check_subject_eq_llong_at_block(
    z3_analyzer_t* analyzer,
    cfg_func_t* function,
    cfg_block_t* block,
    hir_subject_t* subject,
    long long value
);
z3_result_t Z3_check_subject_ne_llong(z3_analyzer_t* analyzer, cfg_func_t* function, hir_subject_t* subject, long long value);
z3_result_t Z3_check_subject_ne_llong_at_block(
    z3_analyzer_t* analyzer,
    cfg_func_t* function,
    cfg_block_t* block,
    hir_subject_t* subject,
    long long value
);
z3_result_t Z3_check_subject_gt_llong(z3_analyzer_t* analyzer, cfg_func_t* function, hir_subject_t* subject, long long value);
z3_result_t Z3_check_subject_ge_llong(z3_analyzer_t* analyzer, cfg_func_t* function, hir_subject_t* subject, long long value);
z3_result_t Z3_check_subject_lt_llong(z3_analyzer_t* analyzer, cfg_func_t* function, hir_subject_t* subject, long long value);
z3_result_t Z3_check_subject_le_llong(z3_analyzer_t* analyzer, cfg_func_t* function, hir_subject_t* subject, long long value);
z3_result_t Z3_check_subject_gt_llong_at_block(
    z3_analyzer_t* analyzer,
    cfg_func_t* function,
    cfg_block_t* block,
    hir_subject_t* subject,
    long long value
);
z3_result_t Z3_check_subject_ge_llong_at_block(
    z3_analyzer_t* analyzer,
    cfg_func_t* function,
    cfg_block_t* block,
    hir_subject_t* subject,
    long long value
);
z3_result_t Z3_check_subject_lt_llong_at_block(
    z3_analyzer_t* analyzer,
    cfg_func_t* function,
    cfg_block_t* block,
    hir_subject_t* subject,
    long long value
);
z3_result_t Z3_check_subject_le_llong_at_block(
    z3_analyzer_t* analyzer,
    cfg_func_t* function,
    cfg_block_t* block,
    hir_subject_t* subject,
    long long value
);
z3_result_t Z3_check_subjects_equal(z3_analyzer_t* analyzer, cfg_func_t* function, hir_subject_t* lhs, hir_subject_t* rhs);
z3_result_t Z3_check_subjects_not_equal(z3_analyzer_t* analyzer, cfg_func_t* function, hir_subject_t* lhs, hir_subject_t* rhs);

z3_result_t Z3_check_subject_truth(z3_analyzer_t* analyzer, cfg_func_t* function, hir_subject_t* subject);
z3_result_t Z3_is_subject_always_true(z3_analyzer_t* analyzer, cfg_func_t* function, hir_subject_t* subject);
z3_result_t Z3_is_subject_maybe_true(z3_analyzer_t* analyzer, cfg_func_t* function, hir_subject_t* subject);
z3_result_t Z3_is_subject_always_false(z3_analyzer_t* analyzer, cfg_func_t* function, hir_subject_t* subject);
z3_result_t Z3_is_subject_maybe_false(z3_analyzer_t* analyzer, cfg_func_t* function, hir_subject_t* subject);

z3_result_t Z3_is_subject_maybe_zero(z3_analyzer_t* analyzer, cfg_func_t* function, hir_subject_t* subject);
z3_result_t Z3_is_subject_always_zero(z3_analyzer_t* analyzer, cfg_func_t* function, hir_subject_t* subject);
z3_result_t Z3_is_subject_always_nonzero(z3_analyzer_t* analyzer, cfg_func_t* function, hir_subject_t* subject);
z3_result_t Z3_is_subject_always_greater_llong(z3_analyzer_t* analyzer, cfg_func_t* function, hir_subject_t* subject, long long value);
z3_result_t Z3_is_subject_maybe_greater_llong(z3_analyzer_t* analyzer, cfg_func_t* function, hir_subject_t* subject, long long value);
z3_result_t Z3_is_subject_always_greater_or_equal_llong(z3_analyzer_t* analyzer, cfg_func_t* function, hir_subject_t* subject, long long value);
z3_result_t Z3_is_subject_maybe_greater_or_equal_llong(z3_analyzer_t* analyzer, cfg_func_t* function, hir_subject_t* subject, long long value);
z3_result_t Z3_is_subject_always_less_llong(z3_analyzer_t* analyzer, cfg_func_t* function, hir_subject_t* subject, long long value);
z3_result_t Z3_is_subject_maybe_less_llong(z3_analyzer_t* analyzer, cfg_func_t* function, hir_subject_t* subject, long long value);
z3_result_t Z3_is_subject_always_less_or_equal_llong(z3_analyzer_t* analyzer, cfg_func_t* function, hir_subject_t* subject, long long value);
z3_result_t Z3_is_subject_maybe_less_or_equal_llong(z3_analyzer_t* analyzer, cfg_func_t* function, hir_subject_t* subject, long long value);
z3_result_t Z3_is_divisor_maybe_zero(z3_analyzer_t* analyzer, cfg_func_t* function, hir_subject_t* divisor);

z3_result_t Z3_is_label_reachable(z3_analyzer_t* analyzer, cfg_func_t* function, long l_id);
z3_result_t Z3_is_block_reachable(z3_analyzer_t* analyzer, cfg_func_t* function, cfg_block_t* block);
z3_result_t Z3_is_edge_feasible(z3_analyzer_t* analyzer, cfg_func_t* function, cfg_block_t* src, cfg_block_t* dst);

#endif
