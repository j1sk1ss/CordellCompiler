#include <generator.h>

int x86_64_generate_ptr_load(ast_node_t* node, FILE* output, gen_ctx_t* ctx) {
    if (!node->token) return 0;
    switch (node->token->t_type) {
        case UNKNOWN_CHAR_VALUE:
        case UNKNOWN_NUMERIC_TOKEN: break; /* Constant load */
        case LONG_VARIABLE_TOKEN:
        case INT_VARIABLE_TOKEN:
        case SHORT_VARIABLE_TOKEN:
        case CHAR_VARIABLE_TOKEN: break; /* Variable load */
        case ARR_VARIABLE_TOKEN:
        case STR_VARIABLE_TOKEN: break; /* Stack array load load */
    }

    return 1;
}

int x86_64_generate_load(ast_node_t* node, FILE* output, gen_ctx_t* ctx) {
    if (!node->token) return 0;
    switch (node->token->t_type) {
        case UNKNOWN_CHAR_VALUE:
        case UNKNOWN_NUMERIC_TOKEN: break; /* Constant load */
        case LONG_VARIABLE_TOKEN:
        case INT_VARIABLE_TOKEN:
        case SHORT_VARIABLE_TOKEN:
        case CHAR_VARIABLE_TOKEN: break; /* Variable load */
        case ARR_VARIABLE_TOKEN:
        case STR_VARIABLE_TOKEN: break; /* Stack array load load */
    }

    return 1;
}
