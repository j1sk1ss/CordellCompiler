#include <sem/semantic.h>

int SMT_check(ast_node_t* node, sym_tables_t* smt) {
    int result = 1;
    result = SMT_check_ro(node)           && result;
    result = SMT_check_sizes(node)        && result;
    result = SMT_check_bitness(node, smt) && result;
    result = SMT_check_rettype(node)      && result;
    result = SMT_check_ownership(node)    && result;
    result = SMT_check_heap_usage(node)   && result;
    return result;
}
