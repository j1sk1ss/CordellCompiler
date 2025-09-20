#include <hir/hirgen/hirgen.h>

int HIR_generate_store_block(ast_node_t* node, hir_ctx_t* ctx) {
    if (!node->token) return 0;
    if (VRS_isptr(node->token)) {
        if (node->child) goto indexing;
        else {
            if (!node->token->flags.dref) {
            }
            else {
            }
        }

        return 1;
    }

    switch (node->token->t_type) {
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
            ast_node_t* off = node->child;
            if (off) {
            }
            else {
            }

            break;
        }
        default: break;
    }

    return 1;
}