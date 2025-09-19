#include <ir/x86_64_gnu_nasm/x86_64_irgen.h>

static int _strdeclaration(ast_node_t* node, ir_gen_t* g, ir_ctx_t* ctx) {
    ast_node_t* name_node = node->child;
    ast_node_t* val_node  = name_node->sibling;

    char* s = val_node->token->value;
    int base_off = name_node->sinfo.offset;
    while (*s) {
        IR_BLOCK2(ctx, iMOV, IR_SUBJ_OFF(base_off--, 1), IR_SUBJ_CNST(*s));
        s++;
    }

    IR_BLOCK2(ctx, iMOV, IR_SUBJ_OFF(base_off--, 1), IR_SUBJ_CNST(0));
    return 1;
}

static int _arrdeclaration(ast_node_t* node, ir_gen_t* g, ir_ctx_t* ctx) {
    ast_node_t* name_node    = node->child;
    ast_node_t* size_node    = name_node->sibling;
    ast_node_t* el_size_node = size_node->sibling;
    ast_node_t* elems_node   = el_size_node->sibling;

    if (name_node->token->flags.heap) {
        print_debug(
            "Heap allocation, var=%s in scope=%i, [rbp - %i]", 
            name_node->token->value, name_node->sinfo.s_id, node->sinfo.offset
        );
        
        g->elemegen(size_node, g, ctx);
        IR_BLOCK1(ctx, PUSH, IR_SUBJ_REG(RAX, 8));
        IR_BLOCK2(ctx, iMOV, IR_SUBJ_REG(RAX, 8), IR_SUBJ_CNST(12));
        IR_BLOCK2(ctx, iMOV, IR_SUBJ_REG(RDI, 8), IR_SUBJ_CNST(0));
        IR_BLOCK0(ctx, SYSC);
        IR_BLOCK2(ctx, iMOV, IR_SUBJ_VAR(node), IR_SUBJ_REG(RAX, 8));
        IR_BLOCK2(ctx, iMOV, IR_SUBJ_REG(RDI, 8), IR_SUBJ_REG(RAX, 8));
        IR_BLOCK1(ctx, POP, IR_SUBJ_REG(RBX, 8));
        IR_BLOCK2(ctx, iADD, IR_SUBJ_REG(RAX, 8), IR_SUBJ_REG(RBX, 8));
        IR_BLOCK2(ctx, iMOV, IR_SUBJ_REG(RDI, 8), IR_SUBJ_REG(RAX, 8));
        IR_BLOCK2(ctx, iMOV, IR_SUBJ_REG(RAX, 8), IR_SUBJ_CNST(12));
        IR_BLOCK0(ctx, SYSC);

        if (scope_id_top(&ctx->heap) != name_node->sinfo.s_id) {
            scope_push(&ctx->heap, name_node->sinfo.s_id, node->sinfo.offset);
        }

        return 1;
    }

    array_info_t arr_info = { .el_size = 1 };
    if (ART_get_info(name_node->token->value, name_node->sinfo.s_id, &arr_info, ctx->synt->symtb.arrs)) {
        int base_off = node->sinfo.offset;
        for (ast_node_t* t = elems_node; t; t = t->sibling) {
            g->elemegen(t, g, ctx);

            switch (arr_info.el_size) {
                case 1:  IR_BLOCK2(ctx, iMOV, IR_SUBJ_OFF(base_off, arr_info.el_size), IR_SUBJ_REG(AL, 1));  break;
                case 2:  IR_BLOCK2(ctx, iMOV, IR_SUBJ_OFF(base_off, arr_info.el_size), IR_SUBJ_REG(AX, 2));  break;
                case 4:  IR_BLOCK2(ctx, iMOV, IR_SUBJ_OFF(base_off, arr_info.el_size), IR_SUBJ_REG(EAX, 4)); break;
                default: IR_BLOCK2(ctx, iMOV, IR_SUBJ_OFF(base_off, arr_info.el_size), IR_SUBJ_REG(RAX, 8)); break;
            }

            base_off -= arr_info.el_size;
        }
    }

    return 1;
}

static int _starr_declaration(ast_node_t* node, ir_gen_t* g, ir_ctx_t* ctx) {
    if (node->token->t_type == ARRAY_TYPE_TOKEN)    return _arrdeclaration(node, g, ctx);
    else if (node->token->t_type == STR_TYPE_TOKEN) return _strdeclaration(node, g, ctx);
    return 1;
}

int IR_generate_declaration_block(ast_node_t* node, ir_gen_t* g, ir_ctx_t* ctx) {
    ast_node_t* name_node = node->child;
    if (!VRS_instack(name_node->token)) return 0;
    if (!VRS_one_slot(name_node->token)) {
        return _starr_declaration(node, g, ctx);
    }

    ast_node_t* val_node = name_node->sibling;
    g->elemegen(val_node, g, ctx);

    switch (VRS_variable_bitness(name_node->token, 1)) {
        case 8: IR_BLOCK2(ctx, iMOV, IR_SUBJ_VAR(name_node), IR_SUBJ_REG(AL, 1)); break;
        case 16: IR_BLOCK2(ctx, iMOV, IR_SUBJ_VAR(name_node), IR_SUBJ_REG(AX, 2)); break;
        case 32: IR_BLOCK2(ctx, iMOV, IR_SUBJ_VAR(name_node), IR_SUBJ_REG(EAX, 4)); break;
        default: IR_BLOCK2(ctx, iMOV, IR_SUBJ_VAR(name_node), IR_SUBJ_REG(RAX, 8)); break;
    }

    return 1;
}
