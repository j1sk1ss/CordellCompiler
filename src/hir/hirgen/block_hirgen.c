#include <hir/hirgens/hirgens.h>

static hir_subject_t* _generation_handler(ast_node_t* node, hir_ctx_t* ctx, sym_table_t* smt) {
    if (!node || !node->token) return NULL;
    if (
        TKN_isoperand(node->token) && node->token->t_type != ASSIGN_TOKEN
    ) return HIR_generate_operand(node, ctx, smt);
    switch (node->token->t_type) {
        case CALL_TOKEN:            return HIR_generate_funccall(node, ctx, smt, 1);
        case SYSCALL_TOKEN:         return HIR_generate_syscall(node, ctx, smt, 1);
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
        case STRING_VALUE_TOKEN:
        case UNKNOWN_NUMERIC_TOKEN: return HIR_generate_load(node, ctx, smt);
        default: break;
    }

    return NULL;
}

hir_subject_t* HIR_generate_elem(ast_node_t* node, hir_ctx_t* ctx, sym_table_t* smt) {
    if (!node) return 0;
    return _generation_handler(node, ctx, smt);
}

static int _navigation_handler(ast_node_t* node, hir_ctx_t* ctx, sym_table_t* smt) {
    if (!node || !node->token) return 0;
    if (TKN_isdecl(node->token)) return HIR_generate_declaration_block(node, ctx, smt);
    if (
        TKN_update_operator(node->token) && node->token->t_type != ASSIGN_TOKEN
    ) return HIR_generate_update_block(node, ctx, smt);
    switch (node->token->t_type) {
        case IF_TOKEN:      HIR_generate_if_block(node, ctx, smt);         break;
        case ASM_TOKEN:     HIR_generate_asmblock(node, ctx, smt);         break;
        case FUNC_TOKEN:    HIR_generate_function_block(node, ctx, smt);   break;
        case EXIT_TOKEN:    HIR_generate_exit_block(node, ctx, smt);       break;
        case CALL_TOKEN:    HIR_generate_funccall(node, ctx, smt, 0);      break;
        case WHILE_TOKEN:   HIR_generate_while_block(node, ctx, smt);      break;
        case START_TOKEN:   HIR_generate_start_block(node, ctx, smt);      break;
        case SWITCH_TOKEN:  HIR_generate_switch_block(node, ctx, smt);     break;
        case RETURN_TOKEN:  HIR_generate_return_block(node, ctx, smt);     break;
        case EXTERN_TOKEN:  HIR_generate_extern_block(node, ctx, smt);     break;
        case IMPORT_TOKEN:  HIR_generate_import_block(node, ctx, smt);     break;
        case ASSIGN_TOKEN:  HIR_generate_assignment_block(node, ctx, smt); break;
        case SYSCALL_TOKEN: HIR_generate_syscall(node, ctx, smt, 0);       break;
        default: break;
    }

    return 1;
}

int HIR_generate_block(ast_node_t* node, hir_ctx_t* ctx, sym_table_t* smt) {
    if (!node) return 0;
    for (ast_node_t* t = node; t; t = t->sibling) {
        if (TKN_isblock(t->token) && (!t->token || t->token->t_type != START_TOKEN)) {
            if (t->token && t->token->t_type == SCOPE_TOKEN) HIR_BLOCK1(ctx, HIR_MKSCOPE, HIR_SUBJ_CONST(t->sinfo.s_id));
            HIR_generate_block(t->child, ctx, smt);
            if (t->token && t->token->t_type == SCOPE_TOKEN) HIR_BLOCK1(ctx, HIR_ENDSCOPE, HIR_SUBJ_CONST(t->sinfo.s_id));
        }

        _navigation_handler(t, ctx, smt);
    }

    return 1;
}
