#include <generator.h>

int get_stack_size(ast_node_t* root, gen_ctx_t* ctx) {
    int size = 0;
    if (!root) return 0;
    for (ast_node_t* t = root->child; t; t = t->sibling) {
#pragma region Navigation
        if (!t->token || t->token->t_type == SCOPE_TOKEN) {
            size = MAX(size, get_stack_size(t, ctx));
            continue;
        }

        if (t->token->t_type == FUNC_TOKEN) continue;
        size = MAX(size, t->info.offset);
    }

    return size;
}

/* Variable just reserve space */
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

/* Variable has init value */
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

int x86_64_generate_data(ast_node_t* node, FILE* output, int section, int bss) {
    if (!node) return 0;
    for (ast_node_t* t = node->child; t; t = t->sibling) {
        if (VRS_isblock(t->token)) {
            x86_64_generate_data(t, output, section, bss);
            continue;
        }

        if (
            VRS_isdecl(t->token) && /* This is declaration */
            ((section == DATA_SECTION)  && t->token->vinfo.glob || /* And this is filter two types */
            (section == RODATA_SECTION) && t->token->vinfo.ro)
        ) {
            if (t->child->sibling && !bss) _generate_init(t, output); /* Has init value */
            else if (!t->child->sibling && bss) _generate_raw(t, output);
        }
        else if (VRS_intext(t->token)) {
            switch (t->token->t_type) {
                case IF_TOKEN:
                case CASE_TOKEN:
                case EXIT_TOKEN:
                case CALL_TOKEN:
                case WHILE_TOKEN:
                case RETURN_TOKEN:
                case SWITCH_TOKEN:
                case SYSCALL_TOKEN:
                case DEFAULT_TOKEN:
                case ARRAY_TYPE_TOKEN: x86_64_generate_data(t, output, section, bss);                          continue;
                case FUNC_TOKEN:       x86_64_generate_data(t->child->sibling->sibling, output, section, bss); continue;
                default: break;
            }
        }
    }

    return 1;
}
