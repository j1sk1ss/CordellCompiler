#include <ast/astgens/astgens.h>

static ast_node_t* _navigation_handler(list_iter_t* it, ast_ctx_t* ctx, sym_table_t* smt) {
    switch (((token_t*)list_iter_current(it))->t_type) {
        case START_TOKEN:           return cpl_parse_start(it, ctx, smt);
        case ASM_TOKEN:             return cpl_parse_asm(it, ctx, smt);
        case OPEN_BLOCK_TOKEN:
        case CLOSE_BLOCK_TOKEN:     return cpl_parse_scope(it, ctx, smt);
        case STR_TYPE_TOKEN:
        case F32_TYPE_TOKEN:
        case F64_TYPE_TOKEN:
        case I8_TYPE_TOKEN:
        case I16_TYPE_TOKEN:      
        case I32_TYPE_TOKEN:
        case I64_TYPE_TOKEN:
        case U8_TYPE_TOKEN:
        case U16_TYPE_TOKEN:
        case U32_TYPE_TOKEN:
        case U64_TYPE_TOKEN:        return cpl_parse_variable_declaration(it, ctx, smt);
        case SWITCH_TOKEN:          return cpl_parse_switch(it, ctx, smt);
        case IF_TOKEN:              
        case WHILE_TOKEN:           return cpl_parse_condop(it, ctx, smt);
        case STR_VARIABLE_TOKEN:
        case ARR_VARIABLE_TOKEN:
        case I8_VARIABLE_TOKEN:
        case I16_VARIABLE_TOKEN:
        case I32_VARIABLE_TOKEN:
        case I64_VARIABLE_TOKEN:
        case F32_VARIABLE_TOKEN:
        case F64_VARIABLE_TOKEN:
        case U8_VARIABLE_TOKEN:
        case U16_VARIABLE_TOKEN:
        case U32_VARIABLE_TOKEN:
        case U64_VARIABLE_TOKEN:
        case UNKNOWN_STRING_TOKEN: 
        case UNKNOWN_NUMERIC_TOKEN: return cpl_parse_expression(it, ctx, smt);
        case SYSCALL_TOKEN:         return cpl_parse_syscall(it, ctx, smt);
        case BREAKPOINT_TOKEN:      return cpl_parse_breakpoint(it);
        case EXTERN_TOKEN:          return cpl_parse_extern(it, ctx, smt);
        case IMPORT_SELECT_TOKEN:   return cpl_parse_import(it, smt);
        case ARRAY_TYPE_TOKEN:      return cpl_parse_array_declaration(it, ctx, smt);
        case CALL_TOKEN:            return cpl_parse_funccall(it, ctx, smt);
        case FUNC_TOKEN:            return cpl_parse_function(it, ctx, smt);
        case EXIT_TOKEN:
        case RETURN_TOKEN:          return cpl_parse_rexit(it, ctx, smt);
        default:                    return NULL;
    }
}

ast_node_t* cpl_parse_block(list_iter_t* it, ast_ctx_t* ctx, sym_table_t* smt, token_type_t ex) {
    ast_node_t* node = AST_create_node(NULL);
    if (!node) return NULL;

    while (list_iter_current(it) && ((token_t*)list_iter_current(it))->t_type != ex) {
        ast_node_t* block = _navigation_handler(it, ctx, smt);
        if (block) AST_add_node(node, block);
        else if (!forward_token(it, 1)) break;
    }

    return node;
}
