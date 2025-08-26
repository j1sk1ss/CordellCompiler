#include <generator.h>

int x86_64_generate_ptr_store(ast_node_t* node, FILE* output, gen_ctx_t* ctx) {
    if (!node->token) return 0;
    switch (node->token->t_type) {
        case LONG_VARIABLE_TOKEN:
        case INT_VARIABLE_TOKEN:
        case SHORT_VARIABLE_TOKEN:
        case CHAR_VARIABLE_TOKEN: break; /* Variable store */
        case ARR_VARIABLE_TOKEN:
        case STR_VARIABLE_TOKEN: break; /* Stack array store */
    }

    return 1;
}

int x86_64_generate_store(ast_node_t* node, FILE* output, gen_ctx_t* ctx) {
    if (!node->token) return 0;
    switch (node->token->t_type) {
        case LONG_VARIABLE_TOKEN:
        case INT_VARIABLE_TOKEN:
        case SHORT_VARIABLE_TOKEN:
        case CHAR_VARIABLE_TOKEN: break; /* Variable store */
        case ARR_VARIABLE_TOKEN:
        case STR_VARIABLE_TOKEN: break; /* Stack array store */
    }

    return 1;
}