/* Main parser logic / navigation */
#include <ast/astgen/astgen.h>

typedef struct {
    ast_node_t*         (*handler)(PARSER_ARGS);
    const token_type_t* types;
    int                 types_count;
} handler_t;

/* Handler for token type parsing */
#define HANDLER(func, ...)                                                                       \
    {                                                                                            \
        .handler = func,                                                                         \
        .types = (const token_type_t[]){ __VA_ARGS__ },                                          \
        .types_count = (int)sizeof((const token_type_t[]){ __VA_ARGS__ }) / sizeof(token_type_t) \
    }

/* Token handlers. The main point where the parser decide which parser
must be invoked for the provided token.
Note: ! If you're extending the parser, add a new handler here ! */
static const handler_t handlers[] = {
    HANDLER(cpl_parse_structdef,         STRUCT_TOKEN),
    HANDLER(cpl_parse_start,             START_TOKEN),
    HANDLER(cpl_parse_asm,               ASM_TOKEN),
    HANDLER(cpl_parse_scope,             OPEN_BLOCK_TOKEN),
    HANDLER(cpl_parse_switch,            SWITCH_TOKEN),
    HANDLER(cpl_parse_if,                IF_TOKEN),
    HANDLER(cpl_parse_while,             WHILE_TOKEN),
    HANDLER(cpl_parse_loop,              LOOP_TOKEN),
    HANDLER(cpl_parse_break,             BREAK_TOKEN),
    HANDLER(cpl_parse_syscall,           SYSCALL_TOKEN),
    HANDLER(cpl_parse_breakpoint,        BREAKPOINT_TOKEN),
    HANDLER(cpl_parse_extern,            EXTERN_TOKEN),
    HANDLER(cpl_parse_import,            IMPORT_SELECT_TOKEN),
    HANDLER(cpl_parse_funccall,          CALL_TOKEN),
    HANDLER(cpl_parse_poparg,            POPARG_TOKEN),
    HANDLER(cpl_parse_function,          FUNC_TOKEN),
    HANDLER(cpl_parse_exit,              EXIT_TOKEN),
    HANDLER(cpl_parse_return,            RETURN_TOKEN),
    HANDLER(cpl_parse_array_declaration, ARRAY_TYPE_TOKEN),
    HANDLER(
        cpl_parse_variable_declaration,
        STR_TYPE_TOKEN, F32_TYPE_TOKEN, F64_TYPE_TOKEN,
        I8_TYPE_TOKEN, I16_TYPE_TOKEN, I32_TYPE_TOKEN, I64_TYPE_TOKEN,
        U8_TYPE_TOKEN, U16_TYPE_TOKEN, U32_TYPE_TOKEN, U64_TYPE_TOKEN
    ),
    HANDLER(
        cpl_parse_expression,
        VARIABLE_TOKEN,
        NEGATIVE_TOKEN,
        REF_TYPE_TOKEN, DREF_TYPE_TOKEN,
        STR_VARIABLE_TOKEN, ARR_VARIABLE_TOKEN,
        I8_VARIABLE_TOKEN, I16_VARIABLE_TOKEN, I32_VARIABLE_TOKEN, I64_VARIABLE_TOKEN,
        F32_VARIABLE_TOKEN, F64_VARIABLE_TOKEN,
        U8_VARIABLE_TOKEN, U16_VARIABLE_TOKEN, U32_VARIABLE_TOKEN, U64_VARIABLE_TOKEN,
        OPEN_BRACKET_TOKEN,
        UNKNOWN_STRING_TOKEN,
        UNKNOWN_FLOAT_NUMERIC_TOKEN,
        UNKNOWN_NUMERIC_TOKEN
    ),
};

/*
Parsers collection navigation.
Params:
    - `it` - Current iterator.
    - `ctx` - AST context.
    - `smt` - Symtable.

Returns an AST node.
*/
static ast_node_t* _navigation_handler(PARSER_ARGS) {
    carry = 0;
    for (int i = 0; i < (int)(sizeof(handlers) / sizeof(handlers[0])); i++) {
        for (int j = 0; j < handlers[i].types_count; j++) {
            if (handlers[i].types[j] == CURRENT_TOKEN->t_type) {
                return handlers[i].handler(it, ctx, smt, carry);
            }
        }
    }

    return NULL;
}

ast_node_t* cpl_parse_element(PARSER_ARGS) {
    return _navigation_handler(it, ctx, smt, carry);
}

ast_node_t* cpl_parse_block(PARSER_ARGS) {
    SAVE_TOKEN_POINT;

    ast_node_t* node = AST_create_node_bt(CREATE_SCOPE_TOKEN);
    if (!node) {
        PARSE_ERROR("Can't create a basic block for the scope block!");
        RESTORE_TOKEN_POINT;
        return NULL;
    }

    while (CURRENT_TOKEN && CURRENT_TOKEN->t_type != carry) {
        ast_node_t* block = cpl_parse_element(it, ctx, smt, carry);
        if (block) AST_add_node(node, block);  /* If we parse succesfully, add a product to the body */
        else if (!forward_token(it, 1)) break; /* If there is a error, proceed the next token        */
    }

    forward_token(it, 1); /* Move from the parser */
    return node;
}
