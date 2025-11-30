#include <prep/markup.h>

typedef struct {
    char*        value;
    token_type_t type;
} markup_token_t;

static const markup_token_t _lexems[] = {
    /* Special single place tokens. */
    { .value = IMPORT_SELECT_COMMAND,  .type = IMPORT_SELECT_TOKEN },
    { .value = IMPORT_COMMAND,         .type = IMPORT_TOKEN        },
    { .value = EXTERN_COMMAND,         .type = EXTERN_TOKEN        },
    { .value = START_COMMAND,          .type = START_TOKEN         },
    { .value = EXIT_COMMAND,           .type = EXIT_TOKEN          },
    { .value = RETURN_TYPE_COMMAND,    .type = RETURN_TYPE_TOKEN   },

    /* Bracket tokens. */
    { .value = OPEN_BLOCK,             .type = OPEN_BLOCK_TOKEN    },
    { .value = CLOSE_BLOCK,            .type = CLOSE_BLOCK_TOKEN   },
    { .value = OPEN_INDEX,             .type = OPEN_INDEX_TOKEN    },
    { .value = CLOSE_INDEX,            .type = CLOSE_INDEX_TOKEN   },
    { .value = OPEN_BRACKET,           .type = OPEN_BRACKET_TOKEN  },
    { .value = CLOSE_BRACKET,          .type = CLOSE_BRACKET_TOKEN },

    /* Function and jmp tokens. */
    { .value = EXFUNCTION_COMMAND,     .type = EXFUNC_TOKEN        },
    { .value = FUNCTION_COMMAND,       .type = FUNC_TOKEN          },
    { .value = RETURN_COMMAND,         .type = RETURN_TOKEN        },
    { .value = SYSCALL_COMMAND,        .type = SYSCALL_TOKEN       },
    { .value = ASM_COMMAND,            .type = ASM_TOKEN           },

    /* Variable modifiers */
    { .value = DREF_COMMAND,           .type = DREF_TYPE_TOKEN     },
    { .value = REF_COMMAND,            .type = REF_TYPE_TOKEN      },
    { .value = PTR_COMMAND,            .type = PTR_TYPE_TOKEN      },
    { .value = RO_COMMAND,             .type = RO_TYPE_TOKEN       },
    { .value = GLOB_COMMAND,           .type = GLOB_TYPE_TOKEN     },
    { .value = NEGATIVE_COMMAND,       .type = NEGATIVE_TOKEN      },

    /* Variable tokens. */
    { .value = I0_VARIABLE,            .type = I0_TYPE_TOKEN       },
    { .value = F64_VARIABLE,           .type = F64_TYPE_TOKEN      },
    { .value = F32_VARIABLE,           .type = F32_TYPE_TOKEN      },
    { .value = I64_VARIABLE,           .type = I64_TYPE_TOKEN      },
    { .value = I32_VARIABLE,           .type = I32_TYPE_TOKEN      },
    { .value = I16_VARIABLE,           .type = I16_TYPE_TOKEN      },
    { .value = I8_VARIABLE,            .type = I8_TYPE_TOKEN       },
    { .value = U64_VARIABLE,           .type = U64_TYPE_TOKEN      },
    { .value = U32_VARIABLE,           .type = U32_TYPE_TOKEN      },
    { .value = U16_VARIABLE,           .type = U16_TYPE_TOKEN      },
    { .value = U8_VARIABLE,            .type = U8_TYPE_TOKEN       },
    { .value = STR_VARIABLE,           .type = STR_TYPE_TOKEN      },
    { .value = ARR_VARIABLE,           .type = ARRAY_TYPE_TOKEN    },

    /* Little jump tokens. */
    { .value = SWITCH_COMMAND,         .type = SWITCH_TOKEN        },
    { .value = CASE_COMMAND,           .type = CASE_TOKEN          },
    { .value = DEFAULT_COMMAND,        .type = DEFAULT_TOKEN       },
    { .value = WHILE_COMAND,           .type = WHILE_TOKEN         },
    { .value = IF_COMMAND,             .type = IF_TOKEN            },
    { .value = ELSE_COMMAND,           .type = ELSE_TOKEN          },

    /* Binary operands. */
    { .value = ADDASSIGN_STATEMENT,    .type = ADDASSIGN_TOKEN     },
    { .value = SUBASSIGN_STATEMENT,    .type = SUBASSIGN_TOKEN     },
    { .value = MULASSIGN_STATEMENT,    .type = MULASSIGN_TOKEN     },
    { .value = DIVASSIGN_STATEMENT,    .type = DIVASSIGN_TOKEN     },
    { .value = ASSIGN_STATEMENT,       .type = ASSIGN_TOKEN        },
    { .value = COMPARE_STATEMENT,      .type = COMPARE_TOKEN       },
    { .value = NCOMPARE_STATEMENT,     .type = NCOMPARE_TOKEN      },
    { .value = PLUS_STATEMENT,         .type = PLUS_TOKEN          },
    { .value = MINUS_STATEMENT,        .type = MINUS_TOKEN         },
    { .value = LARGER_STATEMENT,       .type = LARGER_TOKEN        },
    { .value = LARGEREQ_STATEMENT,     .type = LARGEREQ_TOKEN      },
    { .value = LOWER_STATEMENT,        .type = LOWER_TOKEN         },
    { .value = LOWEREQ_STATEMENT,      .type = LOWEREQ_TOKEN       },
    { .value = MULTIPLY_STATEMENT,     .type = MULTIPLY_TOKEN      },
    { .value = DIVIDE_STATEMENT,       .type = DIVIDE_TOKEN        },
    { .value = MODULO_STATEMENT,       .type = MODULO_TOKEN        },
    { .value = BITMOVE_LEFT_STATEMENT, .type = BITMOVE_LEFT_TOKEN  },
    { .value = BITMOVE_RIGHT_STATMENT, .type = BITMOVE_RIGHT_TOKEN },
    { .value = BITAND_STATEMENT,       .type = BITAND_TOKEN        },
    { .value = BITOR_STATEMENT,        .type = BITOR_TOKEN         },
    { .value = BITXOR_STATEMENT,       .type = BITXOR_TOKEN        },
    { .value = AND_STATEMENT,          .type = AND_TOKEN           },
    { .value = OR_STATEMENT,           .type = OR_TOKEN            },

    /* Debug */
    { .value = BREAKPOINT_COMMAND,     .type = BREAKPOINT_TOKEN    },
};

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

    list_iter_t it;
    list_iter_hinit(tkn, &it);
    token_t* curr;
    while ((curr = (token_t*)list_iter_next(&it))) {
        long t;
        if (map_get(&lexems, crc64((unsigned char*)curr->value, str_strlen(curr->value), 0), (void**)&t)) {
            curr->t_type = t;
        }
    }

    return map_free(&lexems);
}

typedef struct {
    char         ro;
    char         ptr;
    char         glob;
    char         ext;
    token_type_t type;
    short        scope;
    char         name[TOKEN_MAX_SIZE];
} variable_t;

typedef struct {
    char         ro;
    char         glob;
    char         ptr;
    char         ext;
    token_type_t ttype;
} markp_ctx;

static int _add_variable(list_t* vars, const char* name, short scope, markp_ctx* ctx) {
    variable_t* v = (variable_t*)mm_malloc(sizeof(variable_t));
    if (!v) return 0;

    str_memset(v, 0, sizeof(variable_t));
    str_strncpy(v->name, name, TOKEN_MAX_SIZE);
    v->scope = scope;
    v->ext   = ctx->ext;
    v->ro    = ctx->ro;
    v->ptr   = ctx->ptr;
    v->glob  = ctx->glob;
    v->type  = ctx->ttype;

    return list_add(vars, v);
}

static inline void _remove_token(list_t* tkns, token_t* tkn) {
    list_remove(tkns, tkn);
    mm_free(tkn);
}

int MRKP_variables(list_t* tkn) {
    int s_id = 0;
    scope_stack_t scope_stack = { .top = -1 };

    markp_ctx curr_ctx = { 0 };

    list_t vars;
    list_init(&vars);

    list_iter_t it;
    list_iter_hinit(tkn, &it);
    token_t* curr;
    while ((curr = (token_t*)list_iter_next(&it))) {
        switch (curr->t_type) {
            case OPEN_BLOCK_TOKEN:  scope_push(&scope_stack, ++s_id, 0); break;
            case CLOSE_BLOCK_TOKEN: scope_pop(&scope_stack);             break;

            case IMPORT_TOKEN: {
                curr = (token_t*)list_iter_next(&it);
                curr_ctx.ttype = CALL_TOKEN;
                while (curr->t_type != DELIMITER_TOKEN) {
                    if (curr->t_type != COMMA_TOKEN) {
                        _add_variable(&vars, curr->value, scope_id_top(&scope_stack), &curr_ctx);
                    }
                    
                    curr = (token_t*)list_iter_next(&it);
                }
                
                break;
            }

            case EXTERN_TOKEN:    curr_ctx.ext  = 1; _remove_token(tkn, curr); break;
            case GLOB_TYPE_TOKEN: curr_ctx.glob = 1; _remove_token(tkn, curr); break;
            case PTR_TYPE_TOKEN:  curr_ctx.ptr  = 1; _remove_token(tkn, curr); break;
            case RO_TYPE_TOKEN:   curr_ctx.ro = 1;   _remove_token(tkn, curr); break;

            case FUNC_TOKEN:
            case EXFUNC_TOKEN:
            case I8_TYPE_TOKEN:
            case U8_TYPE_TOKEN:
            case I16_TYPE_TOKEN:
            case U16_TYPE_TOKEN:
            case I32_TYPE_TOKEN:
            case U32_TYPE_TOKEN:
            case F32_TYPE_TOKEN:
            case I64_TYPE_TOKEN:
            case U64_TYPE_TOKEN:
            case F64_TYPE_TOKEN:
            case STR_TYPE_TOKEN:
            case ARRAY_TYPE_TOKEN: {
                token_t* next = (token_t*)list_iter_current(&it);
                if (next && (next->t_type == UNKNOWN_STRING_TOKEN || next->t_type == UNKNOWN_CHAR_TOKEN)) {
                    switch (curr->t_type) {
                        case FUNC_TOKEN:
                        case EXFUNC_TOKEN: {
                            curr_ctx.ttype  = CALL_TOKEN;
                            next->t_type    = FUNC_NAME_TOKEN;
                            next->flags.ext = curr_ctx.ext;
                            break;
                        }

                        case I8_TYPE_TOKEN:    curr_ctx.ttype = I8_VARIABLE_TOKEN;  break;
                        case U8_TYPE_TOKEN:    curr_ctx.ttype = U8_VARIABLE_TOKEN;  break;
                        case I32_TYPE_TOKEN:   curr_ctx.ttype = I32_VARIABLE_TOKEN; break;
                        case U32_TYPE_TOKEN:   curr_ctx.ttype = U32_VARIABLE_TOKEN; break;
                        case F32_TYPE_TOKEN:   curr_ctx.ttype = F32_VARIABLE_TOKEN; break;
                        case I64_TYPE_TOKEN:   curr_ctx.ttype = I64_VARIABLE_TOKEN; break;
                        case U64_TYPE_TOKEN:   curr_ctx.ttype = U64_VARIABLE_TOKEN; break;
                        case F64_TYPE_TOKEN:   curr_ctx.ttype = F64_VARIABLE_TOKEN; break;
                        case I16_TYPE_TOKEN:   curr_ctx.ttype = I16_VARIABLE_TOKEN; break;
                        case U16_TYPE_TOKEN:   curr_ctx.ttype = U16_VARIABLE_TOKEN; break;
                        case STR_TYPE_TOKEN:   curr_ctx.ttype = STR_VARIABLE_TOKEN; break;
                        case ARRAY_TYPE_TOKEN: curr_ctx.ttype = ARR_VARIABLE_TOKEN; break;
                        default: break;
                    }

                    _add_variable(&vars, next->value, scope_id_top(&scope_stack), &curr_ctx);
                }

                curr->flags.ro   = curr_ctx.ro;
                curr->flags.ptr  = curr_ctx.ptr;
                curr->flags.ext  = curr_ctx.ext;
                curr->flags.glob = curr_ctx.glob;
                curr_ctx.ext     = 0;
                curr_ctx.ro      = 0;
                curr_ctx.ptr     = 0;
                curr_ctx.glob    = 0;
                curr_ctx.ttype   = 0;
                break;
            }

            default: break;
        }
    }

    s_id = 0;
    scope_reset(&scope_stack);
    list_iter_hinit(tkn, &it);
    
    int dref = 0;
    int ref  = 0;
    int neg  = 0;
    while ((curr = (token_t*)list_iter_next(&it))) {
        switch (curr->t_type) {
            case OPEN_BLOCK_TOKEN:  scope_push(&scope_stack, ++s_id, 0); break;
            case CLOSE_BLOCK_TOKEN: scope_pop(&scope_stack);             break;
            case NEGATIVE_TOKEN:  neg  = 1; _remove_token(tkn, curr); continue;
            case DREF_TYPE_TOKEN: dref = 1; _remove_token(tkn, curr); continue;
            case REF_TYPE_TOKEN:  ref  = 1; _remove_token(tkn, curr); continue;
            case UNKNOWN_CHAR_TOKEN:
            case UNKNOWN_STRING_TOKEN: {
                for (int s = scope_stack.top; s >= 0; s--) {
                    int curr_s = scope_stack.data[s].id;

                    list_iter_t vit;
                    list_iter_hinit(&vars, &vit);
                    variable_t* v;
                    while ((v = (variable_t*)list_iter_next(&vit))) {
                        if (!str_strncmp(curr->value, v->name, TOKEN_MAX_SIZE) && v->scope == curr_s) {
                            curr->t_type     = v->type;
                            curr->flags.ext  = v->ext;
                            curr->flags.ro   = v->ro;
                            curr->flags.glob = v->glob;
                            curr->flags.ptr  = v->ptr;
                            curr->flags.ref  = ref;
                            curr->flags.dref = dref;
                            curr->flags.neg  = neg;
                            goto _resolved;
                        }
                    }
                }

                _resolved: {}
                break;
            }

            default: break;
        }

        ref  = 0;
        dref = 0;
        neg  = 0;
    }

    list_free_force(&vars);
    return 1;
}
