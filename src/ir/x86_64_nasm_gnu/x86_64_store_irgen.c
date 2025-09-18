#include <ir/x86_64_gnu_nasm/x86_64_irgen.h>

int IR_generate_store_block(ast_node_t* node, ir_gen_t* g, ir_ctx_t* ctx) {
    if (!node->token) return 0;
    if (VRS_isptr(node->token)) {
        if (node->child) goto indexing;
        else {
            if (!node->token->flags.dref) IR_BLOCK2(ctx, iMOVq, IR_SUBJ_VAR(node), IR_SUBJ_REG(RAX, 8));
            else {
                array_info_t arr_info = { .el_size = 1 };
                ART_get_info(node->token->value, node->sinfo.s_id, &arr_info, ctx->synt->symtb.arrs);
                int elsize = MAX(VRS_variable_bitness(node->token, 0) / 8, arr_info.el_size);
                IR_BLOCK2(ctx, iMOV, IR_SUBJ_REG(RBX, 8), IR_SUBJ_VAR(node));
                switch (elsize) {
                    case 1: IR_BLOCK2(ctx, iMOV, IR_SUBJ_DRFREG(RBX, 1), IR_SUBJ_REG(AL, 1));  break;
                    case 2: IR_BLOCK2(ctx, iMOV, IR_SUBJ_DRFREG(RBX, 2), IR_SUBJ_REG(AX, 2));  break;
                    case 4: IR_BLOCK2(ctx, iMOV, IR_SUBJ_DRFREG(RBX, 4), IR_SUBJ_REG(EAX, 4)); break;
                    case 8: IR_BLOCK2(ctx, iMOV, IR_SUBJ_DRFREG(RBX, 8), IR_SUBJ_REG(RAX, 8)); break;
                }
            }
        }

        return 1;
    }

    switch (node->token->t_type) {
        case F64_VARIABLE_TOKEN:
        case I64_VARIABLE_TOKEN: 
        case U64_VARIABLE_TOKEN:
            IR_BLOCK2(ctx, iMOV, IR_SUBJ_VAR(node), IR_SUBJ_REG(RAX, 8));
        break;
        case F32_VARIABLE_TOKEN:
        case I32_VARIABLE_TOKEN:
        case U32_VARIABLE_TOKEN:  
            IR_BLOCK2(ctx, iMOV, IR_SUBJ_VAR(node), IR_SUBJ_REG(EAX, 4));
        break;
        case I16_VARIABLE_TOKEN:
        case U16_VARIABLE_TOKEN:
            IR_BLOCK2(ctx, iMOV, IR_SUBJ_VAR(node), IR_SUBJ_REG(AX, 2));
        break;
        case I8_VARIABLE_TOKEN:
        case U8_VARIABLE_TOKEN: 
            IR_BLOCK2(ctx, iMOV, IR_SUBJ_VAR(node), IR_SUBJ_REG(AL, 1));
        break;
        case ARR_VARIABLE_TOKEN:
        case STR_VARIABLE_TOKEN: {
indexing: {}
            ast_node_t* off = node->child;
            if (off) {
                IR_BLOCK2(ctx, iMOV, IR_SUBJ_REG(RDX, 8), IR_SUBJ_REG(RAX, 8));

                array_info_t arr_info = { .el_size = 1 };
                ART_get_info(node->token->value, node->sinfo.s_id, &arr_info, ctx->synt->symtb.arrs);
                int elsize = MAX(VRS_variable_bitness(node->token, 0) / 8, arr_info.el_size);

                g->elemegen(off, g, ctx);
                if (elsize > 1) {
                    IR_BLOCK2(ctx, iMUL, IR_SUBJ_REG(RAX, 8), IR_SUBJ_CNST(elsize));
                }

                if (!node->token->flags.ptr && !node->token->flags.heap) {
                    IR_BLOCK2(ctx, LEA, IR_SUBJ_REG(RBX, 8), IR_SUBJ_VAR(node));
                }
                else {
                    IR_BLOCK2(ctx, iMOV, IR_SUBJ_REG(RBX, 8), IR_SUBJ_VAR(node));
                }

                IR_BLOCK2(ctx, iADD, IR_SUBJ_REG(RAX, 8), IR_SUBJ_REG(RBX, 8));
                switch (elsize) {
                    case 1: IR_BLOCK2(ctx, iMOVb, IR_SUBJ_DRFREG(RAX, 8), IR_SUBJ_REG(DL, 1));  break;
                    case 2: IR_BLOCK2(ctx, iMOVw, IR_SUBJ_DRFREG(RAX, 8), IR_SUBJ_REG(DX, 2));  break;
                    case 4: IR_BLOCK2(ctx, iMOVd, IR_SUBJ_DRFREG(RAX, 8), IR_SUBJ_REG(EDX, 4)); break;
                    case 8: IR_BLOCK2(ctx, iMOVq, IR_SUBJ_DRFREG(RAX, 8), IR_SUBJ_REG(RDX, 8)); break;
                }
            }

            break;
        }
        default: break;
    }

    return 1;
}