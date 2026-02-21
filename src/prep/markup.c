#include <prep/markup.h>

typedef struct {
    char*        value;
    token_type_t type;
} markup_token_t;

#define LEXEM(n, t) { .value = n, .type = t }
static const markup_token_t _lexems[] = {
    /* Special single place tokens. */
    LEXEM(IMPORT_SELECT_COMMAND,  IMPORT_SELECT_TOKEN),
    LEXEM(IMPORT_COMMAND,         IMPORT_TOKEN),
    LEXEM(EXTERN_COMMAND,         EXTERN_TOKEN),
    LEXEM(START_COMMAND,          START_TOKEN),
    LEXEM(EXIT_COMMAND,           EXIT_TOKEN),
    LEXEM(RETURN_TYPE_COMMAND,    RETURN_TYPE_TOKEN),

    /* Bracket tokens. */
    LEXEM(OPEN_BLOCK,             OPEN_BLOCK_TOKEN),
    LEXEM(CLOSE_BLOCK,            CLOSE_BLOCK_TOKEN),
    LEXEM(OPEN_INDEX,             OPEN_INDEX_TOKEN),
    LEXEM(CLOSE_INDEX,            CLOSE_INDEX_TOKEN),
    LEXEM(OPEN_BRACKET,           OPEN_BRACKET_TOKEN),
    LEXEM(CLOSE_BRACKET,          CLOSE_BRACKET_TOKEN),

    /* Function and jmp tokens. */
    LEXEM(EXFUNCTION_COMMAND,     EXFUNC_TOKEN),
    LEXEM(FUNCTION_COMMAND,       FUNC_TOKEN),
    LEXEM(RETURN_COMMAND,         RETURN_TOKEN),
    LEXEM(SYSCALL_COMMAND,        SYSCALL_TOKEN),
    LEXEM(ASM_COMMAND,            ASM_TOKEN),
    LEXEM(VAR_ARGUMENTS_COMMAND,  VAR_ARGUMENTS_TOKEN),

    /* Variable modifiers */
    LEXEM(DREF_COMMAND,           DREF_TYPE_TOKEN),
    LEXEM(REF_COMMAND,            REF_TYPE_TOKEN),
    LEXEM(PTR_COMMAND,            PTR_TYPE_TOKEN),
    LEXEM(RO_COMMAND,             RO_TYPE_TOKEN),
    LEXEM(GLOB_COMMAND,           GLOB_TYPE_TOKEN),
    LEXEM(NEGATIVE_COMMAND,       NEGATIVE_TOKEN),

    /* Variable tokens. */
    LEXEM(I0_VARIABLE,            I0_TYPE_TOKEN),
    LEXEM(F64_VARIABLE,           F64_TYPE_TOKEN),
    LEXEM(F32_VARIABLE,           F32_TYPE_TOKEN),
    LEXEM(I64_VARIABLE,           I64_TYPE_TOKEN),
    LEXEM(I32_VARIABLE,           I32_TYPE_TOKEN),
    LEXEM(I16_VARIABLE,           I16_TYPE_TOKEN),
    LEXEM(I8_VARIABLE,            I8_TYPE_TOKEN),
    LEXEM(U64_VARIABLE,           U64_TYPE_TOKEN),
    LEXEM(U32_VARIABLE,           U32_TYPE_TOKEN),
    LEXEM(U16_VARIABLE,           U16_TYPE_TOKEN),
    LEXEM(U8_VARIABLE,            U8_TYPE_TOKEN),
    LEXEM(STR_VARIABLE,           STR_TYPE_TOKEN),
    LEXEM(ARR_VARIABLE,           ARRAY_TYPE_TOKEN),

    /* Cast token */
    LEXEM(CONVERT_COMMAND,        CONVERT_TOKEN),

    /* Little jump tokens. */
    LEXEM(SWITCH_COMMAND,         SWITCH_TOKEN),
    LEXEM(CASE_COMMAND,           CASE_TOKEN),
    LEXEM(DEFAULT_COMMAND,        DEFAULT_TOKEN),
    LEXEM(WHILE_COMAND,           WHILE_TOKEN),
    LEXEM(LOOP_COMMAND,           LOOP_TOKEN),
    LEXEM(BREAK_COMMAND,          BREAK_TOKEN),
    LEXEM(IF_COMMAND,             IF_TOKEN),
    LEXEM(ELSE_COMMAND,           ELSE_TOKEN),
    LEXEM(POPARG_COMMAND,         POPARG_TOKEN),

    /* Binary operands. */
    LEXEM(ADDASSIGN_STATEMENT,    ADDASSIGN_TOKEN),
    LEXEM(SUBASSIGN_STATEMENT,    SUBASSIGN_TOKEN),
    LEXEM(MULASSIGN_STATEMENT,    MULASSIGN_TOKEN),
    LEXEM(DIVASSIGN_STATEMENT,    DIVASSIGN_TOKEN),
    LEXEM(MODULOASSIGN_STATEMENT, MODULOASSIGN_TOKEN),
    LEXEM(BITANDASSIGN_STATEMENT, BITANDASSIGN_TOKEN),
    LEXEM(BITORASSIGN_STATEMENT,  BITORASSIGN_TOKEN),
    LEXEM(BITXORASSIGN_STATEMENT, BITXORASSIGN_TOKEN),
    LEXEM(ASSIGN_STATEMENT,       ASSIGN_TOKEN),
    LEXEM(COMPARE_STATEMENT,      COMPARE_TOKEN),
    LEXEM(NCOMPARE_STATEMENT,     NCOMPARE_TOKEN),
    LEXEM(PLUS_STATEMENT,         PLUS_TOKEN),
    LEXEM(MINUS_STATEMENT,        MINUS_TOKEN),
    LEXEM(LARGER_STATEMENT,       LARGER_TOKEN),
    LEXEM(LARGEREQ_STATEMENT,     LARGEREQ_TOKEN),
    LEXEM(LOWER_STATEMENT,        LOWER_TOKEN),
    LEXEM(LOWEREQ_STATEMENT,      LOWEREQ_TOKEN),
    LEXEM(MULTIPLY_STATEMENT,     MULTIPLY_TOKEN),
    LEXEM(DIVIDE_STATEMENT,       DIVIDE_TOKEN),
    LEXEM(MODULO_STATEMENT,       MODULO_TOKEN),
    LEXEM(BITMOVE_LEFT_STATEMENT, BITMOVE_LEFT_TOKEN),
    LEXEM(BITMOVE_RIGHT_STATMENT, BITMOVE_RIGHT_TOKEN),
    LEXEM(BITAND_STATEMENT,       BITAND_TOKEN),
    LEXEM(BITOR_STATEMENT,        BITOR_TOKEN),
    LEXEM(BITXOR_STATEMENT,       BITXOR_TOKEN),
    LEXEM(AND_STATEMENT,          AND_TOKEN),
    LEXEM(OR_STATEMENT,           OR_TOKEN),

    /* Debug */
    LEXEM(BREAKPOINT_COMMAND,     BREAKPOINT_TOKEN),
};

/*
Convert the lexem list to the lexem map.
Params:
    - `m` - Output map.

Returns 1 if succeeds.
*/
static int _build_lexems_map(map_t* m) {
    for (int i = 0; i < (int)(sizeof(_lexems) / sizeof(_lexems[0])); i++) {
        map_put(m, crc64((unsigned char*)_lexems[i].value, str_strlen(_lexems[i].value), 0), (void*)_lexems[i].type);
    }

    return 1;
}

int MRKP_mnemonics(list_t* tkn) {
    map_t lexems;
    map_init(&lexems, MAP_NO_CMP);
    _build_lexems_map(&lexems);
    foreach (token_t* curr, tkn) {
        long t;
        if (curr->t_type == STRING_VALUE_TOKEN) continue;
        if (map_get(&lexems, crc64((unsigned char*)curr->body->body, curr->body->size, 0), (void**)&t)) {
            curr->t_type = t;
        }
    }

    return map_free(&lexems);
}

typedef struct {
    token_flags_t modifiers;
    token_type_t  type;
    short         scope;
    string_t*     name;
} variable_t;

/*
Add variable to a list.
Params:
    - `vars` - Variable output list.
    - `name` - Variable name.
    - `scope` - Variable scope.
    - `t` - Variable type.
    - `f` - Variable's flag.

Returns 1 if succeeds.
*/
static int _add_variable(list_t* vars, string_t* name, short scope, token_type_t t, token_flags_t* f) {
    variable_t* v = (variable_t*)mm_malloc(sizeof(variable_t));
    if (!v) return 0;
    str_memset(v, 0, sizeof(variable_t));
    str_memcpy(&v->modifiers, f, sizeof(token_flags_t));
    
    v->name  = name;
    v->scope = scope;
    v->type  = t;
    return list_add(vars, v);
}

/*
Remove token from a list and unload it.
Params:
    - `tkns` - Target list.
    - `tkn` - Targe token.
*/
static inline void _remove_token(list_t* tkns, token_t* tkn) {
    list_remove(tkns, tkn);
    TKN_unload_token(tkn);
}

/*
Apply modifiers in a token list.
How it works:
```cpl
    glob ro ptr i32 a;
```

Will consume the 'glob', the 'ro' and the 'ptr' token, accamulate
information and apply it on the 'i32' token. 
Params:
    - `tkn` - Token list.

Returns 1 if succeeds.
*/
static int _apply_modifiers(list_t* tkn) {
    token_flags_t cflags = { 0 };

    list_iter_t it;
    list_iter_hinit(tkn, &it);
    token_t* curr;
    while ((curr = (token_t*)list_iter_next(&it))) {
        token_t* next = (token_t*)list_iter_current(&it);
        if (next) switch (curr->t_type) {
            case GLOB_TYPE_TOKEN: cflags.glob = 1; _remove_token(tkn, curr); break;
            case PTR_TYPE_TOKEN:  cflags.ptr++;    _remove_token(tkn, curr); break;
            case RO_TYPE_TOKEN:   cflags.ro   = 1; _remove_token(tkn, curr); break;
            default: {
                str_memcpy(&curr->flags, &cflags, sizeof(token_flags_t));
                str_memset(&cflags, 0, sizeof(token_flags_t));
                break;
            }
        }
    }

    return 1;
}

int MRKP_variables(list_t* tkn) {
    _apply_modifiers(tkn);

    int s_id = 0;
    token_type_t ctype = 0;

    list_t vars;
    list_init(&vars);
    sstack_t scope_stack;
    stack_init(&scope_stack);

    list_iter_t it;
    list_iter_hinit(tkn, &it);
    token_t* curr;
    while ((curr = (token_t*)list_iter_next(&it))) {
        switch (curr->t_type) {
            case OPEN_BLOCK_TOKEN:  stack_push(&scope_stack, (void*)((long)++s_id)); break;
            case CLOSE_BLOCK_TOKEN: stack_pop(&scope_stack, NULL);                   break;

            case IMPORT_TOKEN: {
                curr = (token_t*)list_iter_next(&it);
                while (curr && curr->t_type != DELIMITER_TOKEN) {
                    long import_scope;
                    stack_top(&scope_stack, (void**)&import_scope);
                    if (curr->t_type != COMMA_TOKEN) {
                        _add_variable(&vars, curr->body, import_scope, CALL_TOKEN, &curr->flags);
                    }
                    
                    curr = (token_t*)list_iter_next(&it);
                }
                
                break;
            }

            case FUNC_TOKEN:
            case EXFUNC_TOKEN:
            case I8_TYPE_TOKEN:  case U8_TYPE_TOKEN:
            case I16_TYPE_TOKEN: case U16_TYPE_TOKEN:
            case I32_TYPE_TOKEN: case U32_TYPE_TOKEN: case F32_TYPE_TOKEN:
            case I64_TYPE_TOKEN: case U64_TYPE_TOKEN: case F64_TYPE_TOKEN:
            case STR_TYPE_TOKEN:
            case ARRAY_TYPE_TOKEN: {
                token_t* next = (token_t*)list_iter_current(&it);
                if (next && (next->t_type == UNKNOWN_STRING_TOKEN)) {
                    switch (curr->t_type) {
                        case FUNC_TOKEN:
                        case EXFUNC_TOKEN: {
                            ctype           = CALL_TOKEN;
                            next->t_type    = FUNC_NAME_TOKEN;
                            next->flags.ext = curr->flags.ext;
                            break;
                        }

                        case I8_TYPE_TOKEN:  case U8_TYPE_TOKEN:
                        case I16_TYPE_TOKEN: case U16_TYPE_TOKEN:
                        case I32_TYPE_TOKEN: case U32_TYPE_TOKEN: case F32_TYPE_TOKEN:
                        case I64_TYPE_TOKEN: case U64_TYPE_TOKEN: case F64_TYPE_TOKEN:
                        case STR_TYPE_TOKEN: case ARRAY_TYPE_TOKEN: ctype = VARIABLE_TOKEN; break;
                        default: break;
                    }

                    long var_scope;
                    stack_top(&scope_stack, (void**)&var_scope);
                    _add_variable(&vars, next->body, var_scope, ctype, &curr->flags);
                }

                ctype = 0;
                break;
            }

            default: break;
        }
    }

    s_id = 0;
    scope_stack.top = -1;
    
    foreach (token_t* curr, tkn) {
        switch (curr->t_type) {
            case OPEN_BLOCK_TOKEN:  stack_push(&scope_stack, (void*)((long)++s_id)); break;
            case CLOSE_BLOCK_TOKEN: stack_pop(&scope_stack, NULL);                   break;
            case UNKNOWN_STRING_TOKEN: {
                for (int s = scope_stack.top; s >= 0; s--) {
                    short curr_s = (short)((long)scope_stack.data[s].d);
                    foreach (variable_t* v, &vars) {
                        if (curr->body->equals(curr->body, v->name) && v->scope == curr_s) {
                            curr->t_type = v->type;
                            str_memcpy(&curr->flags, &v->modifiers, sizeof(token_flags_t));
                            goto _resolved;
                        }
                    }
                }

                break;
            }

            default: break;
        }
_resolved: {}
    }

    stack_free(&scope_stack);
    list_free_force(&vars);
    return 1;
}
