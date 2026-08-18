#include <ast/astgen/astgen.h>

ast_node_t* cpl_parse_asm(PARSER_ARGS) {
    PARSER_ARGS_USE;
    SAVE_TOKEN_POINT;

    ast_node_t* base = AST_create_node(CURRENT_TOKEN);
    PARSER_DO_OR_THROW(!base, NULL, "Can't create a base for the 'asm' structure!");
    PARSER_DO_OR_THROW(
        !consume_token(it, OPEN_BRACKET_TOKEN), base, 
        "Expected the '(' token while parse of the 'asm' statement!"
    );

    forward_token(it, 1); 
    ast_node_t* args = cpl_parse_call_arguments(it, ctx, smt, 0);
    PARSER_DO_OR_THROW(!args, base, "Error during the 'asm' argument value parsing! Provide variables or values.");
    AST_add_node(base, args);

    ast_node_t* body = AST_create_node_bt(CREATE_SCOPE_TOKEN);
    PARSER_DO_OR_THROW(!body, base, "Can't create a body for the 'asm' structure!");
    AST_add_node(base, body);

    PARSER_DO_OR_THROW(
        !consume_token(it, OPEN_BLOCK_TOKEN), base, 
        "Expected the '{' token while parse of the 'asm' statement!"
    );

    do {
        PARSER_DO_OR_THROW(
            !consume_token(it, STRING_VALUE_TOKEN), base,
            "Expected a string value in the 'asm's body!"
        );

        ast_node_t* arg = AST_create_node(CURRENT_TOKEN);
        if (arg) arg->sinfo.v_id = STTB_add_info(CURRENT_TOKEN->body, STR_RAW_ASM, &smt->s);
        PARSER_DO_OR_THROW(
            !arg || arg->sinfo.v_id == NO_SYMBOL_ID, base, 
            "Can't create a body for the 'asm'-string!"
        );

        AST_add_node(body, arg);
        consume_token(it, COMMA_TOKEN);
    } while (CURRENT_TOKEN && CURRENT_TOKEN->t_type != CLOSE_BLOCK_TOKEN);
    
    forward_token(it, 1);
    return base;
}
