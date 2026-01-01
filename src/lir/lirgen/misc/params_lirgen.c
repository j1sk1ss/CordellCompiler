#include <lir/lirgens/lirgens.h>

int x86_64_pass_params(lir_operation_t op, lir_ctx_t* ctx, list_t* hir_args, list_t* lir_args) {
    int argnum = 0;
    foreach (hir_subject_t* hir_arg, hir_args) {
        lir_subject_t* lir_arg = x86_64_format_variable(hir_arg);
        list_add(lir_args, lir_arg);
        LIR_BLOCK2(ctx, op, lir_arg, LIR_SUBJ_CONST(argnum++));
    }

    return 1;
}