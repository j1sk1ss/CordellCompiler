/* Import logic statements parser
   - from <file> import <name> */
#include <ast/astgen/astgen.h>

ast_node_t* cpl_parse_import(PARSER_ARGS) {
    PARSER_ARGS_USE;
    SAVE_TOKEN_POINT;

    ast_node_t* node = AST_create_node(CURRENT_TOKEN);
    if (!node) {
        PARSE_ERROR("Can't create a base for the import statement!");
        RESTORE_TOKEN_POINT;
        return NULL;
    }
    
    if (!consume_token(it, STRING_VALUE_TOKEN)) {
        PARSE_ERROR("Expected the 'UNKNOWN_STRING_TOKEN' token!");
        AST_unload(node);
        RESTORE_TOKEN_POINT;
        return NULL;
    }
    
    ast_node_t* source_node = AST_create_node(CURRENT_TOKEN);
    if (!source_node) {
        PARSE_ERROR("Can't create a base for the source file!");
        AST_unload(node);
        RESTORE_TOKEN_POINT;
        return NULL;
    }
    
    AST_add_node(node, source_node);
    long sid;
    stack_top(&ctx->scopes.stack, (void**)&sid);
    if (!consume_token(it, IMPORT_TOKEN)) {
        PARSE_ERROR("Expected the 'IMPORT_TOKEN' token!");
        AST_unload(node);
        RESTORE_TOKEN_POINT;
        return NULL;
    }

    forward_token(it, 1);
    while (CURRENT_TOKEN && CURRENT_TOKEN->t_type != DELIMITER_TOKEN) {
        if (CURRENT_TOKEN->t_type == COMMA_TOKEN) {
            forward_token(it, 1);
            continue;
        }

        ast_node_t* import_name = AST_create_node(CURRENT_TOKEN);
        if (!import_name) {
            PARSE_ERROR("Can't create a base for the function name!");
            AST_unload(node);
            RESTORE_TOKEN_POINT;
            return NULL;
        }

        ast_node_t* args_node = AST_create_node_bt(CREATE_SCOPE_TOKEN);
        if (!args_node) {
            PARSE_ERROR("Can't create a base for the function's arguments!");
            AST_unload(node);
            RESTORE_TOKEN_POINT;
            return NULL;
        }

        AST_add_node(import_name, args_node);
        AST_add_node(source_node, import_name);

        import_name->sinfo.v_id = FNTB_add_info(import_name->t->body, 1, 0, 0, sid, args_node, NULL, &smt->f);
        import_name->t->t_type  = FUNC_NAME_TOKEN;
        forward_token(it, 1);
    }
    
    return node;
}
