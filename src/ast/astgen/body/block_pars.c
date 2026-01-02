/* Main parser logic / navigation */
#include <ast/astgen/astgen.h>

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
                                    /* start_function = "start" , "(" , [ param_list ] , ")" , block ; */
        case START_TOKEN:           return cpl_parse_start(it, ctx, smt);
                                    /* asm_block = "asm" , "(" , [ asm_args ] , ")" , "{" , { asm_line } , "}" ;
                                       asm_args  = asm_arg , { "," , asm_arg } ;
                                       asm_arg   = identifier | literal ;
                                       asm_line  = string_literal , [ "," ] ; */
        case ASM_TOKEN:             return cpl_parse_asm(it, ctx, smt);
                                    /* program = "{" , { top_item } , "}" ; */
        case OPEN_BLOCK_TOKEN:      return cpl_parse_scope(it, ctx, smt);
        case STR_TYPE_TOKEN:
        case F32_TYPE_TOKEN:
        case F64_TYPE_TOKEN:
        case I8_TYPE_TOKEN:
        case I16_TYPE_TOKEN:      
        case I32_TYPE_TOKEN:
        case I64_TYPE_TOKEN:
        case U8_TYPE_TOKEN:
        case U16_TYPE_TOKEN:
        case U32_TYPE_TOKEN:        /* type = "f64" | "i64" | "u64" | "f32" | "i32" | "u32" | "i16" | "u16" | "i8" | "u8"
                                                | "str"
                                                | "arr" , "[" , integer_literal , "," , type , "]"
                                                | "ptr" , type ; */
        case U64_TYPE_TOKEN:        return cpl_parse_variable_declaration(it, ctx, smt);
                                    /* switch_statement = "switch" , "(" , expression , ")" , "{" , { case_block } , [ default_block ] , "}" ;
                                       case_block       = "case" , literal , ";" , block ;
                                       default_block    = "default" , block ; */
        case SWITCH_TOKEN:          return cpl_parse_switch(it, ctx, smt);
                                    /* if_statement = "if" , expression , ";" , block , [ "else" , block ] ; */
        case IF_TOKEN:              return cpl_parse_if(it, ctx, smt);
                                    /* while_statement = "while" , expression , ";" , block ; */
        case WHILE_TOKEN:           return cpl_parse_while(it, ctx, smt);
                                    /* loop_statement = "loop", block ; */
        case LOOP_TOKEN:            return cpl_parse_loop(it, ctx, smt);
                                    /* break_statement = "break" , ";" ; */
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
        case UNKNOWN_STRING_TOKEN:  /*  expression   = assign ;
                                        assign       = logical_or , [ "=" , assign ] ;

                                        logical_or   = logical_and , { "||" , logical_and } ;
                                        logical_and  = bit_or      , { "&&" , bit_or } ;

                                        bit_or       = bit_xor     , { "|"  , bit_xor } ;
                                        bit_xor      = bit_and     , { "^"  , bit_and } ;
                                        bit_and      = equality    , { "&"  , equality } ;

                                        equality     = relational  , { ("==" | "!=") , relational } ;
                                        relational   = shift       , { ("<" | "<=" | ">" | ">=") , shift } ;
                                        shift        = add         , { ("<<" | ">>") , add } ;
                                        add          = mul         , { ("+" | "-") , mul } ;
                                        mul          = unary       , { ("*" | "/" | "%") , unary } ;

                                        unary        = unary_op , unary
                                                       | postfix ;

                                        unary_op     = "not" | "+" | "-" ;

                                        postfix      = primary , { postfix_op } ;
                                        postfix_op   = "(" , [ arg_list ] , ")"
                                                        | "[" , expression , { "," , expression } , "]" ;

                                        primary      = literal
                                                       | identifier
                                                       | "(" , expression , ")" ; */
        case UNKNOWN_NUMERIC_TOKEN: return cpl_parse_expression(it, ctx, smt);
                                    /* syscall_statement = "syscall" , "(" , [ syscall_args ] , ")" , ";" ;
                                       syscall_args      = syscall_arg , { "," , syscall_arg } ;
                                       syscall_arg       = identifier | literal ; */
        case SYSCALL_TOKEN:         return cpl_parse_syscall(it, ctx, smt);
                                    /* lis_statement = "lis" , ";" ; */
        case BREAKPOINT_TOKEN:      return cpl_parse_breakpoint(it);
                                    /* extern_op          = "extern" , ( function_prototype | var_prototype ) ;
                                       var_prototype      = type , identifier ;
                                       function_prototype = "exfunc" , identifier ; */
        case EXTERN_TOKEN:          return cpl_parse_extern(it, ctx, smt);
                                    /* import_op      = "from" , string_literal , "import" , [ import_list ] ;
                                       import_list    = import_item , { "," , import_item } ;
                                       import_item    = identifier ; */
        case IMPORT_SELECT_TOKEN:   return cpl_parse_import(it, smt);
                                    /* arr_decl       = "arr" , identifier , "[" , identifier | literal , type ,  "]" , [ "=" , expression | arr_value ] , ";" ;
                                       arr_value      = "{" , [arr_value_list] ,  "}" ;
                                       arr_value_list =  arr_elem , { "," , arr_elem  } ;
                                       arr_elem       = identifier | literal ; */
        case ARRAY_TYPE_TOKEN:      return cpl_parse_array_declaration(it, ctx, smt);
                                    /* postfix        = primary , { postfix_op } ;
                                       postfix_op     = "(" , [ arg_list ] , ")"
                                                   | "[" , expression , { "," , expression } , "]" ; */
        case CALL_TOKEN:            return cpl_parse_funccall(it, ctx, smt);
                                    /* function_def   = "function" , identifier , "(" , [ param_list ] , ")" , "=>" , type , block ; */
        case FUNC_TOKEN:            return cpl_parse_function(it, ctx, smt);
                                    /* exit_statement   = "exit" , expression , ";" ; */
        case EXIT_TOKEN:            return cpl_parse_exit(it, ctx, smt);
                                    /* return_statement = "return" , [ expression ] , ";" ; */
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
