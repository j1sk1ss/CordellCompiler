
#include <syntax.h>

typedef struct {
    char*        value;
    token_type_t type;
} markup_token_t;

static const markup_token_t _markups[] = {
    /* Special single place tokens. */
    { .value = IMPORT_SELECT_COMMAND,  .type = IMPORT_SELECT_TOKEN },
    { .value = IMPORT_COMMAND,         .type = IMPORT_TOKEN        },
    { .value = START_COMMAND,          .type = START_TOKEN         },
    { .value = EXIT_COMMAND,           .type = EXIT_TOKEN          },

    /* Bracket tokens. */
    { .value = OPEN_BLOCK,             .type = OPEN_BLOCK_TOKEN    },
    { .value = CLOSE_BLOCK,            .type = CLOSE_BLOCK_TOKEN   },
    { .value = OPEN_INDEX,             .type = OPEN_INDEX_TOKEN    },
    { .value = CLOSE_INDEX,            .type = CLOSE_INDEX_TOKEN   },
    { .value = OPEN_BRACKET,           .type = OPEN_BRACKET_TOKEN  },
    { .value = CLOSE_BRACKET,          .type = CLOSE_BRACKET_TOKEN },

    /* Function and jmp tokens. */
    { .value = FUNCTION_COMMAND,       .type = FUNC_TOKEN          },
    { .value = RETURN_COMMAND,         .type = RETURN_TOKEN        },
    { .value = SYSCALL_COMMAND,        .type = SYSCALL_TOKEN       },

    /* Variable tokens. */
    { .value = PTR_COMMAND,            .type = PTR_TYPE_TOKEN      },
    { .value = RO_COMMAND,             .type = RO_TYPE_TOKEN       },
    { .value = GLOB_COMMAND,           .type = GLOB_TYPE_TOKEN     },
    { .value = LONG_VARIABLE,          .type = LONG_TYPE_TOKEN     },
    { .value = INT_VARIABLE,           .type = INT_TYPE_TOKEN      },
    { .value = SHORT_VARIABLE,         .type = SHORT_TYPE_TOKEN    },
    { .value = CHAR_VARIABLE,          .type = CHAR_TYPE_TOKEN     },
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
    { .value = ASSIGN_STATEMENT,        .type = ASSIGN_TOKEN         },
    { .value = COMPARE_STATEMENT,      .type = COMPARE_TOKEN       },
    { .value = NCOMPARE_STATEMENT,     .type = NCOMPARE_TOKEN      },
    { .value = PLUS_STATEMENT,         .type = PLUS_TOKEN          },
    { .value = MINUS_STATEMENT,        .type = MINUS_TOKEN         },
    { .value = LARGER_STATEMENT,       .type = LARGER_TOKEN        },
    { .value = LOWER_STATEMENT,        .type = LOWER_TOKEN         },
    { .value = MULTIPLY_STATEMENT,     .type = MULTIPLY_TOKEN      },
    { .value = DIVIDE_STATEMENT,       .type = DIVIDE_TOKEN        },
    { .value = MODULO_STATEMENT,       .type = MODULO_TOKEN        },
    { .value = BITMOVE_LEFT_STATEMENT, .type = BITMOVE_LEFT_TOKEN  },
    { .value = BITMOVE_RIGHT_STATMENT, .type = BITMOVE_RIGHT_TOKEN },
    { .value = BITAND_STATEMENT,       .type = BITAND_TOKEN        },
    { .value = BITOR_STATEMENT,        .type = BITOR_TOKEN         },
    { .value = BITXOR_STATEMENT,       .type = BITXOR_TOKEN        },
    { .value = AND_STATEMENT,          .type = AND_TOKEN           },
    { .value = OR_STATEMENT,           .type = OR_TOKEN            }
};

int MRKP_mnemonics(token_t* head) {
    token_t* curr = head;
    while (curr) {
        for (int i = 0; i < (int)(sizeof(_markups) / sizeof(_markups[0])); i++) {
            if (((char*)curr->value)[0] != _markups[i].value[0]) continue;
            else if (!str_strcmp((char*)curr->value, _markups[i].value)) {
                curr->t_type = _markups[i].type;
            }
        }

        curr = curr->next;
    }

    return 1;
}

typedef struct {
    int           ro;
    int           ptr;
    int           glob;
    token_type_t  type;
    unsigned char name[TOKEN_MAX_SIZE];
} variable_t;

typedef struct {
    char         ro;
    char         glob;
    char         ptr;
    token_type_t ttype;
} markp_ctx;

static int _add_variable(
    variable_t** vars, const char* name, markp_ctx* ctx, int* count
) {
    *vars = mm_realloc(*vars, (*count + 1) * sizeof(variable_t));
    str_memset(&((*vars)[*count]), 0, sizeof(variable_t));
    str_strncpy((char*)((*vars)[*count]).name, name, TOKEN_MAX_SIZE);
    ((*vars)[*count]).ro   = ctx->ro;
    ((*vars)[*count]).ptr  = ctx->ptr;
    ((*vars)[*count]).glob = ctx->glob;
    ((*vars)[*count]).type = ctx->ttype;
    (*count)++;
    return 1;
}

int MRKP_variables(token_t* head) {
    markp_ctx curr_ctx = { 0 };
    token_t* curr = head;
    int var_count = 0;
    variable_t* vars = NULL;

    while (curr) {
        switch (curr->t_type) {
            /* Simple solution is mark all imported functions as call tokens  */
            case IMPORT_TOKEN:
                curr = curr->next;
                curr_ctx.ttype = CALL_TOKEN;
                while (curr->t_type != DELIMITER_TOKEN) {
                    _add_variable(&vars, (const char*)curr->value, &curr_ctx, &var_count);
                    curr = curr->next;
                }
            break;

            case RO_TYPE_TOKEN:   curr_ctx.ro   = 1; break;
            case GLOB_TYPE_TOKEN: curr_ctx.glob = 1; break;
            case PTR_TYPE_TOKEN:  curr_ctx.ptr  = 1; break;

            case FUNC_TOKEN:
            case INT_TYPE_TOKEN:
            case STR_TYPE_TOKEN:
            case LONG_TYPE_TOKEN:
            case CHAR_TYPE_TOKEN:
            case SHORT_TYPE_TOKEN:
            case ARRAY_TYPE_TOKEN: {
                token_t* next = curr->next;
                if (next && (next->t_type == UNKNOWN_STRING_TOKEN || next->t_type == UNKNOWN_CHAR_VALUE)) {
                    switch (curr->t_type) {
                        case FUNC_TOKEN:       curr_ctx.ttype = CALL_TOKEN;           break;
                        case INT_TYPE_TOKEN:   curr_ctx.ttype = INT_VARIABLE_TOKEN;   break;
                        case STR_TYPE_TOKEN:   curr_ctx.ttype = STR_VARIABLE_TOKEN;   break;
                        case LONG_TYPE_TOKEN:  curr_ctx.ttype = LONG_VARIABLE_TOKEN;  break;
                        case CHAR_TYPE_TOKEN:  curr_ctx.ttype = CHAR_VARIABLE_TOKEN;  break;
                        case SHORT_TYPE_TOKEN: curr_ctx.ttype = SHORT_VARIABLE_TOKEN; break;
                        case ARRAY_TYPE_TOKEN: curr_ctx.ttype = ARR_VARIABLE_TOKEN;   break;
                        default: break;
                    }

                    _add_variable(&vars, (const char*)next->value, &curr_ctx, &var_count);
                    curr->ro   = curr_ctx.ro;
                    curr->ptr  = curr_ctx.ptr;
                    curr->glob = curr_ctx.glob;
                }

                curr_ctx.ro    = 0;
                curr_ctx.ptr   = 0;
                curr_ctx.glob  = 0;
                curr_ctx.ttype = 0;
            }
            break;
            default: break;
        }

        curr = curr->next;
    }

    curr = head;
    while (curr) {
        if (curr->t_type == UNKNOWN_STRING_TOKEN || curr->t_type == UNKNOWN_CHAR_VALUE) {
            for (int i = 0; i < var_count; i++) {
                if (!str_strncmp((const char*)curr->value, (const char*)vars[i].name, TOKEN_MAX_SIZE)) {
                    curr->t_type = vars[i].type;
                    curr->ro     = vars[i].ro;
                    curr->glob   = vars[i].glob;
                    curr->ptr    = vars[i].ptr;
                    break;
                }
            }
        }

        curr = curr->next;
    }

    mm_free(vars);
    return 1;
}
