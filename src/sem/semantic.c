#include <sem/semantic.h>

int SEM_perform_ast_check(ast_ctx_t* actx, sym_table_t* smt) {
    ast_walker_t walker;
    ASTWLK_init_ctx(&walker, smt);

    ASTWLK_register_visitor(ASSIGN_NODE, ASTWLKR_ro_assign, &walker);
    ASTWLK_register_visitor(DECLARATION_NODE | ASSIGN_NODE | EXPRESSION_NODE, ASTWLKR_rtype_assign, &walker);
    ASTWLK_register_visitor(DECLARATION_NODE, ASTWLKR_not_init, &walker);
    ASTWLK_register_visitor(DECLARATION_NODE, ASTWLKR_illegal_declaration, &walker);
    ASTWLK_register_visitor(FUNCTION_NODE, ASTWLKR_no_return, &walker);
    ASTWLK_register_visitor(START_NODE, ASTWLKR_no_exit, &walker);
    ASTWLK_register_visitor(CALL_NODE, ASTWLKR_not_enough_args, &walker);
    ASTWLK_register_visitor(CALL_NODE, ASTWLKR_wrong_arg_type, &walker);
    ASTWLK_register_visitor(CALL_NODE, ASTWLKR_unused_rtype, &walker);
    ASTWLK_register_visitor(DEF_ARRAY_NODE, ASTWLKR_illegal_array_access, &walker);

    ASTWLK_walk(actx, &walker);
    ASTWLK_unload_ctx(&walker);
    return 1;
}
