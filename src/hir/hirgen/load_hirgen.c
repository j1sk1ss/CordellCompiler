#include <ir/hir/hir.h>

ir_subject_t* HIR_generate_ptr_load_block(ast_node_t* node, ir_ctx_t* ctx) {
    if (!node->token) return 0;
    if (VRS_isptr(node->token)) {
        if (!node->token->flags.dref) goto indexing; 
        else {
        }
    }
    else {
        switch (node->token->t_type) {
            case UNKNOWN_NUMERIC_TOKEN:
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
            break;
            case ARR_VARIABLE_TOKEN:
            case STR_VARIABLE_TOKEN: {
    indexing: {}
                break;
            }
            default: break;
        }
    }

    if (node->token->flags.neg) {
    }

    return 1;
}

ir_subject_t* HIR_generate_load_block(ast_node_t* node, ir_ctx_t* ctx) {
    if (!node->token) return 0;
    if (node->token->flags.ptr) {
        if (node->child) goto indexing;
        else {
            if (!node->token->flags.dref) {}
            else {
            }
        }
    }
    else {
        switch (node->token->t_type) {
            case UNKNOWN_NUMERIC_TOKEN: 
            break;
            case F64_VARIABLE_TOKEN:
            case I64_VARIABLE_TOKEN:
            case U64_VARIABLE_TOKEN:
            break;
            case F32_VARIABLE_TOKEN:
            case I32_VARIABLE_TOKEN:
            case U32_VARIABLE_TOKEN:
            break;
            case I16_VARIABLE_TOKEN:
            case U16_VARIABLE_TOKEN:
            break;
            case I8_VARIABLE_TOKEN:
            case U8_VARIABLE_TOKEN:
            break;
            case ARR_VARIABLE_TOKEN:
            case STR_VARIABLE_TOKEN: {
    indexing: {}
                break;
            }
            default: break;
        }
    }

    if (node->token->flags.neg) {
    }

    return 1;
}
