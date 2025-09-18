#include <ir/gens/irgen.h>

int IR_generate_store_block(ast_node_t* node, ir_get_t* g, ir_ctx_t* ctx) {
    if (!node->token) return 0;
    if (VRS_isptr(node->token)) {
        if (node->child) goto indexing;
        else {
            if (!node->token->flags.dref) IR_BLOCK2(ctx, iMOVq, IR_SUBJ_VAR(node), IR_SUBJ_REG(RAX));
            else {
                array_info_t arr_info = { .el_size = 1 };
                ART_get_info(node->token->value, node->sinfo.s_id, &arr_info, ctx->synt->symtb.arrs);
                int elsize = MAX(VRS_variable_bitness(node->token, 0) / 8, arr_info.el_size);
                IR_BLOCK2(ctx, iMOV, IR_SUBJ_REG(RBX), IR_SUBJ_VAR(node));
                switch (elsize) {
                    case 1: IR_BLOCK2(ctx, iMOVb, IR_SUBJ_DRFREG(RBX), IR_SUBJ_REG(AL));  break;
                    case 2: IR_BLOCK2(ctx, iMOVb, IR_SUBJ_DRFREG(RBX), IR_SUBJ_REG(AX));  break;
                    case 4: IR_BLOCK2(ctx, iMOVb, IR_SUBJ_DRFREG(RBX), IR_SUBJ_REG(EAX)); break;
                    case 8: IR_BLOCK2(ctx, iMOVb, IR_SUBJ_DRFREG(RBX), IR_SUBJ_REG(RAX)); break;
                }
            }
        }

        return 1;
    }

    switch (node->token->t_type) {
        case F64_VARIABLE_TOKEN:
        case I64_VARIABLE_TOKEN: 
        case U64_VARIABLE_TOKEN:
            IR_BLOCK2(ctx, iMOVq, IR_SUBJ_VAR(node), IR_SUBJ_REG(RAX));
        break;
        case F32_VARIABLE_TOKEN:
        case I32_VARIABLE_TOKEN:
        case U32_VARIABLE_TOKEN:  
            IR_BLOCK2(ctx, iMOVd, IR_SUBJ_VAR(node), IR_SUBJ_REG(EAX));
        break;
        case I16_VARIABLE_TOKEN:
        case U16_VARIABLE_TOKEN:
            IR_BLOCK2(ctx, iMOVw, IR_SUBJ_VAR(node), IR_SUBJ_REG(AX));
        break;
        case I8_VARIABLE_TOKEN:
        case U8_VARIABLE_TOKEN: 
            IR_BLOCK2(ctx, iMOVb, IR_SUBJ_VAR(node), IR_SUBJ_REG(AL));
        break;
        case ARR_VARIABLE_TOKEN:
        case STR_VARIABLE_TOKEN: {
indexing: {}
            ast_node_t* off = node->child;
            if (off) {
                IR_BLOCK2(ctx, iMOV, IR_SUBJ_REG(RDX), IR_SUBJ_REG(RAX));

                array_info_t arr_info = { .el_size = 1 };
                ART_get_info(node->token->value, node->sinfo.s_id, &arr_info, ctx->synt->symtb.arrs);
                int elsize = MAX(VRS_variable_bitness(node->token, 0) / 8, arr_info.el_size);

                g->elemegen(off, g, ctx);
                if (elsize > 1) {
                    IR_BLOCK2(ctx, iMUL, IR_SUBJ_REG(RAX), IR_SUBJ_CNST(elsize));
                }

                if (!node->token->flags.ptr && !node->token->flags.heap) {
                    IR_BLOCK2(ctx, LEA, IR_SUBJ_REG(RBX), IR_SUBJ_VAR(node));
                }
                else {
                    IR_BLOCK2(ctx, iMOV, IR_SUBJ_REG(RBX), IR_SUBJ_VAR(node));
                }

                IR_BLOCK2(ctx, iADD, IR_SUBJ_REG(RAX), IR_SUBJ_REG(RBX));
                switch (elsize) {
                    case 1: IR_BLOCK2(ctx, iMOVb, IR_SUBJ_DRFREG(RAX), IR_SUBJ_REG(DL));  break;
                    case 2: IR_BLOCK2(ctx, iMOVw, IR_SUBJ_DRFREG(RAX), IR_SUBJ_REG(DX));  break;
                    case 4: IR_BLOCK2(ctx, iMOVd, IR_SUBJ_DRFREG(RAX), IR_SUBJ_REG(EAX)); break;
                    case 8: IR_BLOCK2(ctx, iMOVq, IR_SUBJ_DRFREG(RAX), IR_SUBJ_REG(RAX)); break;
                }
            }

            break;
        }
        default: break;
    }

    return 1;
}