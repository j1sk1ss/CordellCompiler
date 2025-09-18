#include <ir/x86_64_gnu_nasm/x86_64_irgen.h>

static int _deref_rax(ir_ctx_t* ctx, int size) {
    switch (size) {
        case 1:  IR_BLOCK2(ctx, iMVZX, IR_SUBJ_REG(RAX, 8), IR_SUBJ_DRFREG(RAX, 1)); break;
        case 2:  IR_BLOCK2(ctx, iMVZX, IR_SUBJ_REG(RAX, 8), IR_SUBJ_DRFREG(RAX, 2)); break;
        case 4:  IR_BLOCK2(ctx, iMOV, IR_SUBJ_REG(EAX, 8), IR_SUBJ_DRFREG(RAX, 8));  break;
        default: IR_BLOCK2(ctx, iMOV, IR_SUBJ_REG(RAX, 8), IR_SUBJ_DRFREG(RAX, 8));  break;
    }

    return 1;
}

static int _indexing(int deref, ast_node_t* node, ir_gen_t* g, ir_ctx_t* ctx) {
    ast_node_t* off = node->child;
    if (!off && !node->token->flags.heap) IR_BLOCK2(ctx, LEA, IR_SUBJ_REG(RAX, 8), IR_SUBJ_VAR(node));
    else if (!off && node->token->flags.heap) IR_BLOCK2(ctx, iMOV, IR_SUBJ_REG(RAX, 8), IR_SUBJ_VAR(node));
    else {
        array_info_t arr_info = { .el_size = 1 };
        ART_get_info(node->token->value, node->sinfo.s_id, &arr_info, ctx->synt->symtb.arrs);
        int elsize = MAX(VRS_variable_bitness(node->token, 0) / 8, arr_info.el_size);
        
        g->elemegen(off, g, ctx);
        if (elsize > 1) {
            IR_BLOCK2(ctx, iMUL, IR_SUBJ_REG(RAX, 8), IR_SUBJ_CNST(elsize));
        }
        
        if (
            !node->token->flags.ptr && !node->token->flags.heap
        ) IR_BLOCK2(ctx, LEA, IR_SUBJ_REG(RBX, 8), IR_SUBJ_VAR(node));
        else IR_BLOCK2(ctx, iMOV, IR_SUBJ_REG(RBX, 8), IR_SUBJ_VAR(node));
        IR_BLOCK2(ctx, iADD, IR_SUBJ_REG(RAX, 8), IR_SUBJ_REG(RBX, 8));
        if (deref) _deref_rax(ctx, elsize);
    }

    return 1;
}

int IR_generate_ptr_load_block(ast_node_t* node, ir_gen_t* g, ir_ctx_t* ctx) {
    if (!node->token) return 0;
    if (VRS_isptr(node->token)) {
        if (!node->token->flags.dref) goto indexing; 
        else {
            array_info_t arr_info = { .el_size = 1 };
            ART_get_info(node->token->value, node->sinfo.s_id, &arr_info, ctx->synt->symtb.arrs);
            IR_BLOCK2(ctx, iMOV, IR_SUBJ_REG(RAX, 8), IR_SUBJ_VAR(node));
            _deref_rax(ctx, MAX(VRS_variable_bitness(node->token, 0) / 8, arr_info.el_size));
        }
    }
    else {
        switch (node->token->t_type) {
            case UNKNOWN_NUMERIC_TOKEN:
                IR_BLOCK2(ctx, iMOV, IR_SUBJ_REG(RAX, 8), IR_SUBJ_STR(4, node->token->value));
            break;
            case I8_VARIABLE_TOKEN:
            case U8_VARIABLE_TOKEN:
            case F64_VARIABLE_TOKEN:
            case I64_VARIABLE_TOKEN:
            case U64_VARIABLE_TOKEN:
            case F32_VARIABLE_TOKEN:
            case I32_VARIABLE_TOKEN:
            case U32_VARIABLE_TOKEN:
            case I16_VARIABLE_TOKEN:
            case U16_VARIABLE_TOKEN:
                IR_BLOCK2(ctx, LEA, IR_SUBJ_REG(RAX, 8), IR_SUBJ_VAR(node));
            break;
            case ARR_VARIABLE_TOKEN:
            case STR_VARIABLE_TOKEN: {
    indexing: {}
                _indexing(0, node, g, ctx);
                break;
            }
            default: break;
        }
    }

    if (node->token->flags.neg) {
        IR_BLOCK2(ctx, TST, IR_SUBJ_REG(RAX, 8), IR_SUBJ_REG(RAX, 8));
        IR_BLOCK1(ctx, SETE, IR_SUBJ_REG(AL, 1));
        IR_BLOCK2(ctx, iMVZX, IR_SUBJ_REG(RAX, 8), IR_SUBJ_REG(AL, 1));
    }

    return 1;
}

int IR_generate_load_block(ast_node_t* node, ir_gen_t* g, ir_ctx_t* ctx) {
    if (!node->token) return 0;
    if (node->token->flags.ptr) {
        if (node->child) goto indexing;
        else {
            if (!node->token->flags.dref) IR_BLOCK2(ctx, iMOV, IR_SUBJ_REG(RAX, 8), IR_SUBJ_VAR(node));
            else {
                array_info_t arr_info = { .el_size = 1 };
                ART_get_info(node->token->value, node->sinfo.s_id, &arr_info, ctx->synt->symtb.arrs);
                IR_BLOCK2(ctx, iMOV, IR_SUBJ_REG(RAX, 8), IR_SUBJ_VAR(node));
                _deref_rax(ctx, MAX(VRS_variable_bitness(node->token, 0) / 8, arr_info.el_size));
            }
        }
    }
    else {
        switch (node->token->t_type) {
            case UNKNOWN_NUMERIC_TOKEN: {
                IR_BLOCK2(ctx, iMOV, IR_SUBJ_REG(RAX, 8), IR_SUBJ_STR(8, node->token->value));
                break;
            }
            case F64_VARIABLE_TOKEN:
            case I64_VARIABLE_TOKEN:
            case U64_VARIABLE_TOKEN:
                IR_BLOCK2(ctx, iMOV, IR_SUBJ_REG(RAX, 8), IR_SUBJ_VAR(node));
            break;
            case F32_VARIABLE_TOKEN:
            case I32_VARIABLE_TOKEN:
            case U32_VARIABLE_TOKEN:
                IR_BLOCK2(ctx, iMOV, IR_SUBJ_REG(EAX, 4), IR_SUBJ_VAR(node));
            break;
            case I16_VARIABLE_TOKEN:
            case U16_VARIABLE_TOKEN:
                IR_BLOCK2(ctx, iMOV, IR_SUBJ_REG(AX, 2), IR_SUBJ_VAR(node));
            break;
            case I8_VARIABLE_TOKEN:
            case U8_VARIABLE_TOKEN:
                IR_BLOCK2(ctx, iMOV, IR_SUBJ_REG(AL, 1), IR_SUBJ_VAR(node));
            break;
            case ARR_VARIABLE_TOKEN:
            case STR_VARIABLE_TOKEN: {
    indexing: {}
                _indexing(1, node, g, ctx);
                break;
            }
            default: break;
        }
    }

    if (node->token->flags.neg) {
        IR_BLOCK2(ctx, TST, IR_SUBJ_REG(RAX, 8), IR_SUBJ_REG(RAX, 8));
        IR_BLOCK1(ctx, SETE, IR_SUBJ_REG(AL, 1));
        IR_BLOCK2(ctx, iMVZX, IR_SUBJ_REG(RAX, 8), IR_SUBJ_REG(AL, 8));
    }

    return 1;
}
