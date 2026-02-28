#include <sem/semantic.h>

int SEM_perform_ast_check(ast_ctx_t* actx, sym_table_t* smt) {
    ast_walker_t walker;
    ASTWLK_init_ctx(&walker, smt);

    /* Low Level */
    ASTWLK_register_visitor(DECLARATION_NODE | ASSIGN_NODE | EXPRESSION_NODE, ASTWLKR_rtype_assign, &walker, ATTENTION_LOW_LEVEL);
    ASTWLK_register_visitor(DECLARATION_NODE, ASTWLKR_not_init, &walker, ATTENTION_LOW_LEVEL);
    ASTWLK_register_visitor(CALL_NODE, ASTWLKR_unused_rtype, &walker, ATTENTION_LOW_LEVEL);
    ASTWLK_register_visitor(FUNCTION_NODE, ASTWLKR_valid_function_name, &walker, ATTENTION_LOW_LEVEL);
    ASTWLK_register_visitor(IF_NODE, ASTWLKR_duplicated_branches, &walker, ATTENTION_LOW_LEVEL);
    ASTWLK_register_visitor(WHILE_NODE, ASTWLKR_inefficient_while, &walker, ATTENTION_LOW_LEVEL);
    ASTWLK_register_visitor(EXPRESSION_NODE, ASTWLKR_unused_expression, &walker, ATTENTION_LOW_LEVEL);
    ASTWLK_register_visitor(DECLARATION_NODE, ASTWLKR_incorrect_align, &walker, ATTENTION_LOW_LEVEL);

    /* Medium Level */
    ASTWLK_register_visitor(DECLARATION_NODE, ASTWLKR_illegal_declaration, &walker, ATTENTION_MEDIUM_LEVEL);
    ASTWLK_register_visitor(FUNCTION_NODE, ASTWLKR_wrong_rtype, &walker, ATTENTION_MEDIUM_LEVEL);
    ASTWLK_register_visitor(TERM_NODE, ASTWLKR_deadcode, &walker, ATTENTION_MEDIUM_LEVEL);
    ASTWLK_register_visitor(EXPRESSION_NODE, ASTWLKR_implict_convertion, &walker, ATTENTION_MEDIUM_LEVEL);
    ASTWLK_register_visitor(BREAK_NODE, ASTWLKR_break_without_statement, &walker, ATTENTION_MEDIUM_LEVEL);
    ASTWLK_register_visitor(REF_NODE, ASTWLKR_ref_to_expression, &walker, ATTENTION_MEDIUM_LEVEL);

    /* High Level */
    ASTWLK_register_visitor(CALL_NODE, ASTWLKR_wrong_arg_type, &walker, ATTENTION_HIGH_LEVEL);

    /* Block Level */
    ASTWLK_register_visitor(ASSIGN_NODE, ASTWLKR_ro_assign, &walker, ATTENTION_BLOCK_LEVEL);
    ASTWLK_register_visitor(FUNCTION_NODE, ASTWLKR_no_return, &walker, ATTENTION_BLOCK_LEVEL);
    ASTWLK_register_visitor(START_NODE, ASTWLKR_no_exit, &walker, ATTENTION_BLOCK_LEVEL);
    ASTWLK_register_visitor(CALL_NODE, ASTWLKR_not_enough_args, &walker, ATTENTION_BLOCK_LEVEL);
    ASTWLK_register_visitor(INDEX_NODE, ASTWLKR_illegal_array_access, &walker, ATTENTION_BLOCK_LEVEL);
    ASTWLK_register_visitor(START_NODE | FUNCTION_NODE, ASTWLKR_wrong_exit, &walker, ATTENTION_BLOCK_LEVEL);
    ASTWLK_register_visitor(CALL_NODE, ASTWLKR_noret_assign, &walker, ATTENTION_BLOCK_LEVEL);

    ASTWLK_walk(actx, &walker);
    ASTWLK_unload_ctx(&walker);
    return 1;
}
