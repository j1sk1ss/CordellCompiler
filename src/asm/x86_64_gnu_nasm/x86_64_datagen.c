#include <asm/x86_64_gnu_nasm/x86_64_asmgen.h>

/* Variable just reserve space */
static int _generate_raw(ast_node_t* entry, FILE* output) {
    if (!entry->child || !entry->token) return 0;
    switch (entry->token->t_type) {
        case I64_TYPE_TOKEN: iprintf(output, "__%s__: resq 1\n", entry->child->token->value); break;
        case I32_TYPE_TOKEN: iprintf(output, "__%s__: resd 1\n", entry->child->token->value); break;
        case I16_TYPE_TOKEN: iprintf(output, "__%s__: resw 1\n", entry->child->token->value); break;
        case I8_TYPE_TOKEN:  iprintf(output, "__%s__: resb 1\n", entry->child->token->value); break;
        case ARRAY_TYPE_TOKEN: {
            ast_node_t* name   = entry->child;
            ast_node_t* size   = name->sibling;
            ast_node_t* t_type = size->sibling;

            char* directive = "resb";
            if (t_type->token->t_type == I16_TYPE_TOKEN)      directive = "resw";
            else if (t_type->token->t_type == I32_TYPE_TOKEN) directive = "resd";
            else if (t_type->token->t_type == I64_TYPE_TOKEN) directive = "resq";
            iprintf(output, "__%s__: %s %s\n", name->token->value, directive, size->token->value);
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
        case STR_TYPE_TOKEN: {
            iprintf(output, "__%s__ db ", entry->child->token->value);
            char* data = entry->child->sibling->token->value;
            while (*data) {
                iprintf(output, "%i,", *data);
                data++;
            }

            iprintf(output, "0\n");
            break;
        }

        case I64_TYPE_TOKEN: iprintf(output, "__%s__ dq %s\n", entry->child->token->value, entry->child->sibling->token->value); break;
        case I32_TYPE_TOKEN: iprintf(output, "__%s__ dd %s\n", entry->child->token->value, entry->child->sibling->token->value); break;
        case I16_TYPE_TOKEN: iprintf(output, "__%s__ dw %s\n", entry->child->token->value, entry->child->sibling->token->value); break;
        case I8_TYPE_TOKEN:  iprintf(output, "__%s__ db %s\n", entry->child->token->value, entry->child->sibling->token->value); break;
        case ARRAY_TYPE_TOKEN: {
            ast_node_t* name   = entry->child;
            ast_node_t* size   = name->sibling;
            ast_node_t* t_type = size->sibling;
            
            char* directive = "db";
            if (t_type->token->t_type == I16_TYPE_TOKEN)      directive = "dw";
            else if (t_type->token->t_type == I32_TYPE_TOKEN) directive = "dd";
            else if (t_type->token->t_type == I64_TYPE_TOKEN) directive = "dq";
            iprintf(output, "__%s__ %s ", name->token->value, directive);
            for (ast_node_t* elem = t_type->sibling; elem; elem = elem->sibling) {
                if (elem->token->t_type == UNKNOWN_NUMERIC_TOKEN) fprintf(output, "%s%s", elem->token->value, elem->sibling ? "," : "\n");
                else { /* Char conversion */
                    int value = elem->token->value[0];
                    fprintf(output, "%i%s", value, elem->sibling ? "," : "\n");
                }
            }

            break;
        }
        default: break;
    }

    return 1;
}

static int _generate_extern(ast_node_t* entry, FILE* output) {
    if (VRS_isdecl(entry->token)) iprintf(output, "extern %s\n", entry->child->token->value);
    else iprintf(output, "extern %s\n", entry->token->value);
    return 1;
}

#define BSS            1
#define NO_BSS         0
#define DATA_SECTION   1
#define RODATA_SECTION 2
#define EXT_SECTION    3
int _generate_data(ast_node_t* node, FILE* output, int section, int bss) {
    if (!node) return 0;
    for (ast_node_t* t = node->child; t; t = t->sibling) {
        if (VRS_isblock(t->token)) {
            _generate_data(t, output, section, bss);
            continue;
        }

        if (
            VRS_isdecl(t->token) && /* This is declaration */
            ((section == DATA_SECTION)  && t->token->flags.glob || /* And this is filter two types */
            (section == RODATA_SECTION) && t->token->flags.ro)
        ) {
            if (t->token->t_type != ARRAY_TYPE_TOKEN) {
                if (t->child->sibling && !bss)      _generate_init(t, output);
                else if (!t->child->sibling && bss) _generate_raw(t, output);
            }
            else {
                if (t->child->sibling->sibling->sibling && !bss)      _generate_init(t, output);
                else if (!t->child->sibling->sibling->sibling && bss) _generate_raw(t, output);
            }
        }
        else if (VRS_instack(t->token)) {
            switch (t->token->t_type) {
                case EXTERN_TOKEN: {
                    if (section == EXT_SECTION) _generate_extern(t->child, output);
                    continue;
                }

                case IF_TOKEN:
                case CASE_TOKEN:
                case FUNC_TOKEN:
                case EXIT_TOKEN:
                case CALL_TOKEN:
                case WHILE_TOKEN:
                case RETURN_TOKEN:
                case SWITCH_TOKEN:
                case SYSCALL_TOKEN:
                case DEFAULT_TOKEN:
                case ARRAY_TYPE_TOKEN: _generate_data(t, output, section, bss); continue;
                default: break;
            }
        }
    }

    return 1;
}

int x86_64_generate_data(ast_node_t* node, FILE* output) {
    _generate_data(node, output, EXT_SECTION, NO_BSS);
    iprintf(output, "section .data\n");
    _generate_data(node, output, DATA_SECTION, NO_BSS);
    iprintf(output, "section .rodata\n");
    _generate_data(node, output, RODATA_SECTION, NO_BSS);
    iprintf(output, "section .bss\n");
    _generate_data(node, output, DATA_SECTION, BSS);
    iprintf(output, "section .text\n");
    return 1;
}
