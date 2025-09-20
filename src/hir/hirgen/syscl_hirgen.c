#include <lir/hir/hir.h>

int HIR_generate_start_block(ast_node_t* node, lir_ctx_t* ctx) {
    return 1;
}

int HIR_generate_exit_block(ast_node_t* node, lir_ctx_t* ctx) {
    return 1;
}

int HIR_generate_syscall_block(ast_node_t* node, lir_ctx_t* ctx) {
    return 1;
}
