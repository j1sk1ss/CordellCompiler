#include <ast/astgen/astgen.h>

DEFINE_PARSER(cpl_parse_breakpoint, {
    ast_node_t* node = AST_create_node(CURRENT_TOKEN);
    PARSER_DO_OR_THROW(!node, NULL, "Can't create a base for a 'lis' statement!");
    
    if (consume_token(it, STRING_VALUE_TOKEN)) {
        ast_node_t* info = AST_create_node(CURRENT_TOKEN);
        PARSER_DO_OR_THROW(!info, node, "Can't create a base for a 'lis' message!");
        PARSER_DO_OR_THROW(
            (info->sinfo.v_id = STTB_add_info(info->t->body, STR_COMMENT, &smt->s)) == NO_SYMBOL_ID, node,
            "Can't register a comment for a 'lis' statement!"
        );

        AST_add_node(node, info);
        forward_token(it, 1);
    }

    forward_token(it, 1);
    return node;
})
