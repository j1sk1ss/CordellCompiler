#include <ast/astgen/astgen.h>

DEFINE_PARSER(cpl_parse_switch, {
    ast_node_t* base = AST_create_node(CURRENT_TOKEN);
    PARSER_DO_OR_THROW(!base, NULL, "Can't create a base for a 'switch' structure!");
 
    stack_top(&ctx->scopes.stack, (void**)&base->sinfo.s_id);
    DUMP_ANNOTATION_TO_NODE(ctx, base);

    forward_token(it, 1);
    ast_node_t* stmt = cpl_parse_expression(it, ctx, smt, 1);
    PARSER_DO_OR_THROW(!stmt, base, "Error during the parsing of the 'switch' statement!");
    AST_add_node(base, stmt);

    ast_node_t* cases_scope = AST_create_node_bt(CREATE_SCOPE_TOKEN);
    PARSER_DO_OR_THROW(!cases_scope, base, "Can't create a base for a case scope!");
    AST_add_node(base, cases_scope);

    PARSER_DO_OR_THROW(
        !consume_token(it, OPEN_BLOCK_TOKEN), base, 
        "Expected the 'OPEN_BLOCK_TOKEN' token during parse of the 'switch' statement!"
    );

    forward_token(it, 1);
    while (
        CURRENT_TOKEN->t_type == CASE_TOKEN    || 
        CURRENT_TOKEN->t_type == DEFAULT_TOKEN ||
        CURRENT_TOKEN->t_type == ANNOTATION_TOKEN 
    ) {
        if (CURRENT_TOKEN->t_type == ANNOTATION_TOKEN) {
            cpl_parse_annot(it, ctx, smt, carry);
            forward_token(it, 1);
            continue;
        }

        ast_node_t* case_node = AST_create_node(CURRENT_TOKEN);
        PARSER_DO_OR_THROW(!case_node, base, "Can't create a base for the case in the 'switch' statement!");
        AST_add_node(cases_scope, case_node);

        DUMP_ANNOTATION_TO_NODE(ctx, case_node);

        if (CURRENT_TOKEN->t_type == CASE_TOKEN) {
            forward_token(it, 1);
            ast_node_t* case_stmt = cpl_parse_expression(it, ctx, smt, 1);
            AST_add_node(case_node, case_stmt);
        }

        ast_node_t* case_body = NULL;
        if (!consume_token(it, OPEN_BLOCK_TOKEN)) case_body = cpl_parse_line_scope(it, ctx, smt, 1);
        else                                      case_body = cpl_parse_scope(it, ctx, smt, 1);

        PARSER_DO_OR_THROW(!case_body, base, "Error during the parsing process for the case!");
        AST_add_node(case_node, case_body);
    }

    forward_token(it, 1);
    return base;
})
