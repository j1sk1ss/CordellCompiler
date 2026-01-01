/* Main parser logic / navigation */
#include <ast/astgen/astgen.h>

ast_node_t* cpl_parse_scope(list_iter_t* it, ast_ctx_t* ctx, sym_table_t* smt) {
    if (CURRENT_TOKEN->t_type != OPEN_BLOCK_TOKEN) {
        print_error("This isn't a block's open token!");
        return NULL;
    }

    forward_token(it, 1);
    stack_push(&ctx->scopes.stack, (void*)((long)++ctx->scopes.s_id));
    ast_node_t* node = cpl_parse_block(it, ctx, smt, CLOSE_BLOCK_TOKEN);
    if (node) {
        stack_top(&ctx->scopes.stack, (void**)&node->sinfo.s_id);
        forward_token(it, 1);
    }

    stack_pop(&ctx->scopes.stack, NULL);
    return node;
}

/*
Parsers collection navigation.
Note: New parsers must be registered Here!
Params:
    - `it` - Current iterator.
    - `ctx` - AST context.
    - `smt` - Symtable.

Returns an AST node.
*/
static ast_node_t* _navigation_handler(list_iter_t* it, ast_ctx_t* ctx, sym_table_t* smt) {
    switch (CURRENT_TOKEN->t_type) {
                                    /* start( (opt: <type> <name>) ) { <content } */
        case START_TOKEN:           return cpl_parse_start(it, ctx, smt);
                                    /* asm( <stmt> ) { <content> }                */
        case ASM_TOKEN:             return cpl_parse_asm(it, ctx, smt);
        case OPEN_BLOCK_TOKEN:      /* { <content> }                              */
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
        case U32_TYPE_TOKEN:        /* <type> <name> (opt: <= val>)               */
        case U64_TYPE_TOKEN:        return cpl_parse_variable_declaration(it, ctx, smt);
                                    /* 
                                    switch ( <stmt> ) { 
                                        <case <val>/<default> 
                                    } 
                                    */
        case SWITCH_TOKEN:          return cpl_parse_switch(it, ctx, smt);
                                    /* if ( <stmt> ) { <content> }                */
        case IF_TOKEN:              return cpl_parse_if(it, ctx, smt);
                                    /* while ( <stmt> ) { <content> }             */
        case WHILE_TOKEN:           return cpl_parse_while(it, ctx, smt);
                                    /* loop { <content> }                         */
        case LOOP_TOKEN:            return cpl_parse_loop(it, ctx, smt);
                                    /* break                                      */
        case BREAK_TOKEN:           return cpl_parse_break(it);
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
        case UNKNOWN_STRING_TOKEN:  /* <stmt>                                     */
        case UNKNOWN_NUMERIC_TOKEN: return cpl_parse_expression(it, ctx, smt);
                                    /* syscall( <stmt> )                          */
        case SYSCALL_TOKEN:         return cpl_parse_syscall(it, ctx, smt);
                                    /* lis                                        */
        case BREAKPOINT_TOKEN:      return cpl_parse_breakpoint(it);
                                    /* extern <type> <name>                       */
        case EXTERN_TOKEN:          return cpl_parse_extern(it, ctx, smt);
                                    /* from <file> import <name>                  */
        case IMPORT_SELECT_TOKEN:   return cpl_parse_import(it, smt);
                                    /* arr <name>[<type>, <size>] (opt: = {})     */
        case ARRAY_TYPE_TOKEN:      return cpl_parse_array_declaration(it, ctx, smt);
                                    /* <func_name>( <stmt> )                      */
        case CALL_TOKEN:            return cpl_parse_funccall(it, ctx, smt);
                                    /*
                                    function <name>((opt: <type> <name>)) {
                                        <content>
                                    }
                                    */
        case FUNC_TOKEN:            return cpl_parse_function(it, ctx, smt);
                                    /* exit <stmt>                                */
        case EXIT_TOKEN:            return cpl_parse_exit(it, ctx, smt);
                                    /* return <stmt>                              */
        case RETURN_TOKEN:          return cpl_parse_return(it, ctx, smt);
        default:                    return NULL;
    }
}

ast_node_t* cpl_parse_block(list_iter_t* it, ast_ctx_t* ctx, sym_table_t* smt, token_type_t ex) {
    SAVE_TOKEN_POINT;

    ast_node_t* node = AST_create_node(TKN_create_token(SCOPE_TOKEN, NULL, CURRENT_TOKEN->lnum));
    if (!node) {
        print_error("Can't create a basic block for a scope!");
        RESTORE_TOKEN_POINT;
        return NULL;
    }

    node->bt = node->t;
    while (CURRENT_TOKEN && CURRENT_TOKEN->t_type != ex) {
        ast_node_t* block = _navigation_handler(it, ctx, smt);
        if (block) AST_add_node(node, block);
        else if (!forward_token(it, 1)) break;
    }

    return node;
}
