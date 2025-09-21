#include <sem/semantic.h>

int SMT_check(syntax_ctx_t* sctx) {
    int result = 1;
    result = SMT_check_ro(sctx)         && result;
    result = SMT_check_sizes(sctx)      && result;
    result = SMT_check_bitness(sctx)    && result;
    result = SMT_check_rettype(sctx)    && result;
    result = SMT_check_ownership(sctx)  && result;
    result = SMT_check_heap_usage(sctx) && result;
    return result;
}
