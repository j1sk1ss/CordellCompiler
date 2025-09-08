#include <semantic.h>

int SMT_check(ast_node_t* node) {
    int result = 1;
    result = SMT_check_ro(node) && result;
    result = SMT_check_sizes(node) && result;
    result = SMT_check_bitness(node) && result;
    return result;
}
