#include <ast/astgen/astgen.h>

DEFINE_PARSER(cpl_parse_sizeof, {
    ast_node_t* base = AST_create_node(CURRENT_TOKEN);
    PARSER_DO_OR_THROW(!base, NULL, "Can't create a base for the sizeof command!");
    PARSER_DO_OR_THROW(!consume_token(it, OPEN_BRACKET_TOKEN), base, "Expected the 'OPEN_BRACKET_TOKEN'!");
    
    forward_token(it, 1);
    ast_node_t* body = cpl_parse_expression(it, ctx, smt, 1);
    PARSER_DO_OR_THROW(!body, base, "Error during the sizeof body parse! sizeof(<exp>)!");

    AST_add_node(base, body);
    body->sinfo.t_id = type_lookup(body->t, ctx, smt);

    forward_token(it, 1);
    return base;
})
