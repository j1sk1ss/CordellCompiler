#include <hir/hir.h>

static hir_subject_t* _navigation_handler(ast_node_t* node, hir_ctx_t* ctx) {
    if (!node || !node->token) return 0;
    if (VRS_isdecl(node->token)) return HIR_generate_declaration_block(node, ctx);
    if (
        VRS_isoperand(node->token) && node->token->t_type != ASSIGN_TOKEN
    ) return HIR_generate_operand_block(node, ctx);
    switch (node->token->t_type) {
        case IF_TOKEN:           return HIR_generate_if_block(node, ctx);
        case ASM_TOKEN:          return HIR_generate_asmblock(node, ctx);
        case CALL_TOKEN:         return HIR_generate_funccall_block(node, ctx);
        case FUNC_TOKEN:         return HIR_generate_function_block(node, ctx);
        case EXIT_TOKEN:         return HIR_generate_exit_block(node, ctx);
        case WHILE_TOKEN:        return HIR_generate_while_block(node, ctx);
        case START_TOKEN:        return HIR_generate_start_block(node, ctx);
        case SWITCH_TOKEN:       return HIR_generate_switch_block(node, ctx);
        case RETURN_TOKEN:       return HIR_generate_return_block(node, ctx);
        case ASSIGN_TOKEN:       return HIR_generate_assignment_block(node, ctx);
        case SYSCALL_TOKEN:      return HIR_generate_syscall_block(node, ctx);
        case I8_VARIABLE_TOKEN:
        case U8_VARIABLE_TOKEN:
        case I16_VARIABLE_TOKEN:
        case U16_VARIABLE_TOKEN:
        case I32_VARIABLE_TOKEN:
        case U32_VARIABLE_TOKEN:
        case F32_VARIABLE_TOKEN:
        case I64_VARIABLE_TOKEN:
        case U64_VARIABLE_TOKEN:
        case F64_VARIABLE_TOKEN:
        case ARR_VARIABLE_TOKEN:
        case STR_VARIABLE_TOKEN: 
        case UNKNOWN_NUMERIC_TOKEN:
            if (node->token->flags.ref) return HIR_generate_ptr_load_block(node, ctx);
        return HIR_generate_load_block(node, ctx);
        default: break;
    }

    return 1;
}

hir_subject_t* HIR_generate_elem_block(ast_node_t* node, hir_ctx_t* ctx) {
    if (!node) return 0;
    if (VRS_isblock(node->token)) return HIR_generate_elem_block(node->child, ctx);
    return _navigation_handler(node, ctx);
}

int HIR_generate_block(ast_node_t* node, hir_ctx_t* ctx) {
    if (!node) return 0;
    for (ast_node_t* t = node; t; t = t->sibling) {
        if (VRS_isblock(t->token) && (!t->token || t->token->t_type != START_TOKEN)) {
            HIR_generate_block(t->child, ctx);
        }

        _navigation_handler(t, ctx);
    }

    return 1;
}
