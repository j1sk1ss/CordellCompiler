/* The 'start' keyword parser
   - start */
#include <ast/astgen/astgen.h>

ast_node_t* cpl_parse_start(PARSER_ARGS) {
    PARSER_ARGS_USE;
    SAVE_TOKEN_POINT;

    ast_node_t* node = AST_create_node(CURRENT_TOKEN);
    if (!node) {
        PARSE_ERROR("Can't create a base for the '%s' statement!", START_COMMAND);
        RESTORE_TOKEN_POINT;
        return NULL;
    }

    if (!consume_token(it, OPEN_BRACKET_TOKEN)) {
        PARSE_ERROR("Expected the 'OPEN_BRACKET_TOKEN' token during a parse of the '%s' statement!", START_COMMAND);
        AST_unload(node);
        RESTORE_TOKEN_POINT;
        return NULL;
    }

    forward_token(it, 1);
    if (!cpl_parse_funcdef_args(it, ctx, smt, (long)node)) {
        PARSE_ERROR("Can't parse start's arguments!");
        AST_unload(node);
        RESTORE_TOKEN_POINT;
        return NULL;
    }

    if (!consume_token(it, OPEN_BLOCK_TOKEN)) {
        PARSE_ERROR("Expected the 'OPEN_BLOCK_TOKEN' in a body of the '%s' statement! %s( ... ) { ... }!", START_COMMAND, START_COMMAND);
        AST_unload(node);
        RESTORE_TOKEN_POINT;
        return NULL;
    }

    ast_node_t* body = cpl_parse_scope(it, ctx, smt, 1);
    if (!body) {
        PARSE_ERROR("Error during the parsing of the '%s' body!", START_COMMAND);
        AST_unload(node);
        RESTORE_TOKEN_POINT;
        return NULL;
    }

    AST_add_node(node, body);

    string_t* main_name = create_string(ctx->fentry);
    if (FNTB_get_info(main_name, -1, NULL, &smt->f)) {
        PARSE_ERROR("The main function already exists!");
        AST_unload(node);
        destroy_string(main_name);
        RESTORE_TOKEN_POINT;
        return NULL;
    }

    stack_top(&ctx->scopes.stack, (void**)&node->sinfo.s_id);
    node->sinfo.v_id = FNTB_add_info(main_name, 1, 0, 1, node->sinfo.s_id, node, NULL, &smt->f);
    destroy_string(main_name);
    return node;
}
