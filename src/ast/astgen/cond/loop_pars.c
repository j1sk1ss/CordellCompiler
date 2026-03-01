#include <ast/astgen/astgen.h>

ast_node_t* cpl_parse_loop(PARSER_ARGS) {
    PARSER_ARGS_USE;
    SAVE_TOKEN_POINT;

    ast_node_t* base = AST_create_node(CURRENT_TOKEN);
    if (!base) {
        PARSE_ERROR("Can't create a base for the '%s' statement!", LOOP_COMMAND);
        RESTORE_TOKEN_POINT;
        return NULL;
    }
    
    stack_top(&ctx->scopes.stack, (void**)&base->sinfo.s_id);

    ast_node_t* body = NULL;
    if (!consume_token(it, OPEN_BLOCK_TOKEN)) body = cpl_parse_line_scope(it, ctx, smt, 1);
    else body = cpl_parse_scope(it, ctx, smt, 1);
    if (body) AST_add_node(base, body);
    else {
        PARSE_ERROR("Error during parsing in the '%s' statement body!", LOOP_COMMAND);
        AST_unload(base);
        RESTORE_TOKEN_POINT;
        return NULL;
    }
    
    annotation_t* annot;
    while (stack_pop(&ctx->annots, (void**)&annot)) {
        list_add(&base->annots, annot);
    }

    return base;
}
