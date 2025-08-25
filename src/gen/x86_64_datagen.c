#include <generator.h>

static int _generate_raw(ast_node_t* entry, FILE* output) {
    if (!entry->child || !entry->token) return 0;
    switch (entry->token->t_type) {
        case LONG_TYPE_TOKEN:  iprintf(output, "__%s__: resq 1\n", entry->child->token->value); break;
        case INT_TYPE_TOKEN:   iprintf(output, "__%s__: resd 1\n", entry->child->token->value); break;
        case SHORT_TYPE_TOKEN: iprintf(output, "__%s__: resw 1\n", entry->child->token->value); break;
        case CHAR_TYPE_TOKEN:  iprintf(output, "__%s__: resb 1\n", entry->child->token->value); break;
        case ARRAY_TYPE_TOKEN: {
            ast_node_t* size   = entry->child;
            ast_node_t* t_type = size->sibling;
            ast_node_t* name   = t_type->sibling;
            if (!name->sibling) {
                char* directive = "resb";
                if (t_type->token->t_type == SHORT_TYPE_TOKEN)     directive = "resw";
                else if (t_type->token->t_type == INT_TYPE_TOKEN)  directive = "resd";
                else if (t_type->token->t_type == LONG_TYPE_TOKEN) directive = "resq";
                iprintf(output, "__%s__: %s %s\n", name->token->value, directive, size->token->value);
            }
        }
        break;
        default: break;
    }

    return 1;
}

static int _generate_init(ast_node_t* entry, FILE* output) {
    if (!entry->child || !entry->token) return 0;
    switch (entry->token->t_type) {
        case STR_TYPE_TOKEN:   iprintf(output, "__%s__ db '%s', 0\n", entry->child->token->value, entry->child->sibling->token->value); break;
        case LONG_TYPE_TOKEN:  iprintf(output, "__%s__ dq %s\n", entry->child->token->value, entry->child->sibling->token->value); break;
        case INT_TYPE_TOKEN:   iprintf(output, "__%s__ dd %s\n", entry->child->token->value, entry->child->sibling->token->value); break;
        case SHORT_TYPE_TOKEN: iprintf(output, "__%s__ dw %s\n", entry->child->token->value, entry->child->sibling->token->value); break;
        case CHAR_TYPE_TOKEN:  iprintf(output, "__%s__ db %s\n", entry->child->token->value, entry->child->sibling->token->value); break;
        case ARRAY_TYPE_TOKEN: {
            ast_node_t* size   = entry->child;
            ast_node_t* t_type = size->sibling;
            ast_node_t* name   = t_type->sibling;
            if (name->sibling) {
                char* directive = "db";
                if (t_type->token->t_type == SHORT_TYPE_TOKEN)     directive = "dw";
                else if (t_type->token->t_type == INT_TYPE_TOKEN)  directive = "dd";
                else if (t_type->token->t_type == LONG_TYPE_TOKEN) directive = "dq";
                iprintf(output, "%s %s ", name->token->value, directive);
                for (ast_node_t* elem = name->sibling; elem; elem = elem->sibling) {
                    if (elem->token->t_type == UNKNOWN_NUMERIC_TOKEN) fprintf(output, "%s%s", elem->token->value, elem->sibling ? "," : "\n");
                    else { /* Char conversion */
                        int value = elem->token->value[0];
                        fprintf(output, "%i%s", value, elem->sibling ? "," : "\n");
                    }
                }
            }

            break;
        }
        default: break;
    }

    return 1;
}

#define DATA_SECTION   1
#define RODATA_SECTION 2
int x86_64_generate_data(syntax_ctx_t* sctx, FILE* output, int section, int bss) {
    variable_info_t* curr = sctx->vars->h;
    while (curr) {
        if (
            (section == DATA_SECTION)   && curr->glob || 
            (section == RODATA_SECTION) && curr->ro
        ) {
            if (!bss) _generate_init(child, output);
            else _generate_raw(child, output);
        }

        curr = curr->next;
    }

    return 1;
}
