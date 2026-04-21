#include <ast/astgen/astgen.h>

ast_node_t* cpl_parse_exit(PARSER_ARGS) {
    PARSER_ARGS_USE;
    SAVE_TOKEN_POINT;

    ast_node_t* base = AST_create_node(CURRENT_TOKEN);
    if (!base) {
        PARSE_ERROR("Can't create the base for the '%s' statement!", EXIT_COMMAND);
        RESTORE_TOKEN_POINT;
        return NULL;
    }
    
    stack_top(&ctx->scopes.stack, (void**)&base->sinfo.s_id);

    forward_token(it, 1);
    ast_node_t* value = cpl_parse_expression(it, ctx, smt, 1);
    if (value) AST_add_node(base, value);
    else {
        PARSE_ERROR("Error during a parse of the '%s' statement! '%s' <stmt>!", EXIT_COMMAND, EXIT_COMMAND);
        AST_unload(base);
        RESTORE_TOKEN_POINT;
        return NULL;
    }

    forward_token(it, 1);
    return base;
}