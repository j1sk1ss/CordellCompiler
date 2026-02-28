#include <hir/hirgens/hirgens.h>

/*
Generation element handler. The main purpose is handling of the 'return' AST nodes.
Note: The 'return' AST node is a node that returns something. For instance:
        1. function call
        2. syscall
        3. operands
        4. store operations
        -> are the 'return' nodes.
Note 2: If the provided AST node doesn't return anything (for example a syscall without value saving),
        this function will return the 'NULL' value.
Note 3: Typically this is a 'slave' AST node.
Params:
    - `node` - Input AST node.
    - `ctx` - HIR context.
    - `smt` - Symtable.

Returns generated value from the AST node or the 'NULL' value.
*/
static hir_subject_t* _generation_handler(ast_node_t* node, hir_ctx_t* ctx, sym_table_t* smt) {
    if (!node || !node->t) return NULL;
    switch (node->t->t_type) {
        case CALLING_TOKEN:
        case CALL_TOKEN:            return HIR_generate_funccall(node, ctx, smt, 1);
        case POPARG_TOKEN:          return HIR_generate_poparg(ctx, smt);
        case SYSCALL_TOKEN:         return HIR_generate_syscall(node, ctx, smt, 1);
        case CONVERT_TOKEN:         return HIR_generate_explconv(node, ctx, smt);
        case NEGATIVE_TOKEN:        return HIR_generate_neg(node, ctx, smt);
        case REF_TYPE_TOKEN:        return HIR_generate_ref(node, ctx, smt);
        case DREF_TYPE_TOKEN:       return HIR_generate_dref(node, ctx, smt, NULL);
        case INDEXATION_TOKEN:      return HIR_generate_load_indexation(node, ctx, smt);
        /* We skip assign nodes above given the next logic, 
           where we generate the special load sequence */
        case CALL_ADDR_TOKEN:
        case I0_VARIABLE_TOKEN:
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
        case UNKNOWN_NUMERIC_TOKEN:
        case UNKNOWN_FLOAT_NUMERIC_TOKEN: return HIR_generate_load(node, ctx, smt);
        default: break;
    }

    if (TKN_update_operator(node->t)) return HIR_generate_update_block(node, ctx, smt, 1);
    else if (TKN_isoperand(node->t))  return HIR_generate_operand(node, ctx, smt);
    return NULL;
}

hir_subject_t* HIR_generate_elem(ast_node_t* node, hir_ctx_t* ctx, sym_table_t* smt) {
    return _generation_handler(node, ctx, smt);
}

/*
Select the generator depends on the provided node's type.
Note: Typically this is a 'master' AST node.
Params:
    - `node` - AST node.
    - `ctx` - HIR context.
    - `smt` - Symtable.

Returns 1 if succeeds, otherwise will return 0.
*/
static int _navigation_handler(ast_node_t* node, hir_ctx_t* ctx, sym_table_t* smt) {
    if (!node || !node->t) return 0;
    switch (node->t->t_type) {
        case IF_TOKEN:         return HIR_generate_if_block(node, ctx, smt);
        case ASM_TOKEN:        return HIR_generate_asmblock(node, ctx, smt);
        case FUNC_TOKEN:       return HIR_generate_function_block(node, ctx, smt);
        case EXIT_TOKEN:       return HIR_generate_exit_block(node, ctx, smt);
        case CALLING_TOKEN:
        case CALL_TOKEN:       return HIR_generate_funccall(node, ctx, smt, 0);
        case LOOP_TOKEN:       return HIR_generate_loop_block(node, ctx, smt);
        case BREAK_TOKEN:      return HIR_generate_break_block(ctx);
        case WHILE_TOKEN:      return HIR_generate_while_block(node, ctx, smt);
        case START_TOKEN:      return HIR_generate_start_block(node, ctx, smt);
        case SWITCH_TOKEN:     return HIR_generate_switch_block(node, ctx, smt);
        case RETURN_TOKEN:     return HIR_generate_return_block(node, ctx, smt);
        case EXTERN_TOKEN:     return HIR_generate_extern_block(node, ctx);
        case IMPORT_TOKEN:     return HIR_generate_import_block(node, ctx);
        case ASSIGN_TOKEN:     return HIR_generate_assignment_block(node, ctx, smt);
        case SYSCALL_TOKEN:    return HIR_generate_syscall(node, ctx, smt, 0);
        case BREAKPOINT_TOKEN: return HIR_generate_breakpoint_block(node, ctx);
        default: break;
    }

    if (TKN_is_decl(node->t))          return HIR_generate_declaration_block(node, ctx, smt);
    if (TKN_update_operator(node->t)) (void)HIR_generate_update_block(node, ctx, smt, 0);
    return 1;
}

/*
We need to save scopes from the AST.
Fot this purpose we create a scope command when we've met a scope AST node.
Note: If the provided AST node isn't a scope node, 
      this function wont create a scope HIR command.
Params:
    - `t` - AST node.
    - `ctx` - HIR context.
    - `op` - 'HIR_MKSCOPE' or 'HIR_ENDSCOPE' command.
*/
static inline void _insert_scope(ast_node_t* t, hir_ctx_t* ctx, hir_operation_t op) {
    if (t->t && t->t->t_type == SCOPE_TOKEN) HIR_BLOCK1(ctx, op, HIR_SUBJ_CONST(t->sinfo.s_id));
}

int HIR_generate_block(ast_node_t* node, hir_ctx_t* ctx, sym_table_t* smt) {
    if (!node) return 0;
    for (ast_node_t* t = node; t; t = t->siblings.n) {
        if (TKN_isblock(t->t) && (!t->t || t->t->t_type != START_TOKEN)) {
            _insert_scope(t, ctx, HIR_MKSCOPE);
            HIR_generate_block(t->c, ctx, smt);
            _insert_scope(t, ctx, HIR_ENDSCOPE);
        }

        _navigation_handler(t, ctx, smt);
    }

    return 1;
}
