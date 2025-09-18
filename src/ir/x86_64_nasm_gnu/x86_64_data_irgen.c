#include <ir/x86_64_gnu_nasm/x86_64_gnu_nasm_irgen.h>

int get_stack_size(ast_node_t* root) {
    if (!root) return 0;

    int size = 0;
    for (ast_node_t* t = root; t; t = t->sibling) {
        if (VRS_isblock(t->token)) {
            size = MAX(size, get_stack_size(t->child));
            continue;
        }

        if (t->token && t->token->t_type == FUNC_TOKEN) continue;
        if (!VRS_instack(t->token)) continue;
        size = MAX(MAX(t->sinfo.offset, get_stack_size(t->child)), size);
    }

    return size;
}

static int _generate_raw(ast_node_t* entry, ir_ctx_t* ctx) {
    if (!entry->child || !entry->token) return 0;
    switch (entry->token->t_type) {
        case F64_TYPE_TOKEN:
        case U64_TYPE_TOKEN:
        case I64_TYPE_TOKEN:
        case U32_TYPE_TOKEN:
        case I32_TYPE_TOKEN:
        case U16_TYPE_TOKEN:
        case I16_TYPE_TOKEN:
        case U8_TYPE_TOKEN:
        case I8_TYPE_TOKEN: {
            IR_BLOCK2(
                ctx, RESV, IR_SUBJ_STR(VRS_variable_bitness(entry->token, 1) / 8, entry->child->token->value),
                IR_SUBJ_CNST(1)
            );
            
            break;
        }

        case ARRAY_TYPE_TOKEN: {
            ast_node_t* name   = entry->child;
            ast_node_t* size   = name->sibling;
            ast_node_t* t_type = size->sibling;
            IR_BLOCK2(
                ctx, RESV, IR_SUBJ_STR(VRS_variable_bitness(t_type->token, 1) / 8, name->token->value),
                IR_SUBJ_STR(8, size->token->value)
            );

            break;
        }
        default: break;
    }

    return 1;
}

static int _generate_init(ast_node_t* entry, ir_ctx_t* ctx) {
    if (!entry->child || !entry->token) return 0;
    switch (entry->token->t_type) {
        case STR_TYPE_TOKEN: {
            // TODO:
            // iprintf(output, "__%s__ db ", entry->child->token->value);
            // char* data = entry->child->sibling->token->value;
            // while (*data) {
            //     iprintf(output, "%i,", *data);
            //     data++;
            // }

            // iprintf(output, "0\n");
            break;
        }

        case F64_TYPE_TOKEN:
        case U64_TYPE_TOKEN:
        case I64_TYPE_TOKEN:
        case U32_TYPE_TOKEN:
        case I32_TYPE_TOKEN:
        case U16_TYPE_TOKEN:
        case I16_TYPE_TOKEN:
        case U8_TYPE_TOKEN:
        case I8_TYPE_TOKEN: {
            IR_BLOCK2(
                ctx, VDCL, IR_SUBJ_STR(VRS_variable_bitness(entry->token, 1) / 8, entry->child->token->value),
                IR_SUBJ_STR(8, entry->child->sibling->token->value)
            );
            
            break;
        }

        case ARRAY_TYPE_TOKEN: {
            ast_node_t* name   = entry->child;
            ast_node_t* size   = name->sibling;
            ast_node_t* t_type = size->sibling;
            IR_BLOCK2(
                ctx, VDCL, IR_SUBJ_STR(VRS_variable_bitness(t_type->token, 1) / 8, name->token->value),
                IR_SUBJ_STR(8, size->token->value)
            );
            
            // TODO:
            // char* directive = "db";
            // if (t_type->token->t_type == I16_TYPE_TOKEN)      directive = "dw";
            // else if (t_type->token->t_type == I32_TYPE_TOKEN) directive = "dd";
            // else if (t_type->token->t_type == I64_TYPE_TOKEN) directive = "dq";
            // iprintf(output, "__%s__ %s ", name->token->value, directive);
            // for (ast_node_t* elem = t_type->sibling; elem; elem = elem->sibling) {
            //     fprintf(output, "%s%s", elem->token->value, elem->sibling ? "," : "\n");
            // }

            break;
        }
        default: break;
    }

    return 1;
}

static int _generate_extern(ast_node_t* entry, ir_ctx_t* ctx) {
    if (VRS_isdecl(entry->token)) IR_BLOCK1(ctx, OEXT, IR_SUBJ_STR(8, entry->child->token->value));
    else IR_BLOCK1(ctx, OEXT, IR_SUBJ_STR(8, entry->token->value));
    return 1;
}

int IR_generate_data_block(ast_node_t* node, int section, int bss, ir_gen_t* g, ir_ctx_t* ctx) {
    if (!node) return 0;
    for (ast_node_t* t = node->child; t; t = t->sibling) {
        if (VRS_isblock(t->token)) {
            g->datagen(t, section, bss, g, ctx);
            continue;
        }

        if (
            VRS_isdecl(t->token) &&
            ((section == DATA_SECTION)  && t->token->flags.glob ||
            (section == RODATA_SECTION) && t->token->flags.ro)
        ) {
            if (t->token->t_type != ARRAY_TYPE_TOKEN) {
                if (t->child->sibling && !bss)      _generate_init(t, ctx);
                else if (!t->child->sibling && bss) _generate_raw(t, ctx);
            }
            else {
                if (t->child->sibling->sibling->sibling && !bss)      _generate_init(t, ctx);
                else if (!t->child->sibling->sibling->sibling && bss) _generate_raw(t, ctx);
            }
        }
        else if (VRS_instack(t->token)) {
            switch (t->token->t_type) {
                case EXTERN_TOKEN: {
                    if (section == EXT_SECTION) _generate_extern(t->child, ctx);
                    continue;
                }

                default: break;
            }
        }

        g->datagen(t, section, bss, g, ctx);
    }

    return 1;
}
