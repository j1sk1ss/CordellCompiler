#include <sem/semantic.h>

int SEM_perform_ast_check(ast_ctx_t* actx) {
    ast_walker_t walker;
    ASTWLK_init_ctx(&walker);

    ASTWLK_register_visitor(ASSIGN_NODE, ASTWLKR_ro_assign, &walker);
    ASTWLK_register_visitor(ASSIGN_NODE, ASTWLKR_rtype_assign, &walker);

    ASTWLK_walk(actx, &walker);
    ASTWLK_unload_ctx(&walker);
    return 1;
}
