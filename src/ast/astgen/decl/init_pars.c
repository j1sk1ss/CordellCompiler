#include <ast/astgen/astgen.h>

ast_node_t* cpl_parse_declaration_value(PARSER_ARGS) {
    PARSER_ARGS_USE;
    SAVE_TOKEN_POINT;

    long long const_length = -1;
    if (carry) const_length = *((long long*)carry);
    ast_node_t* base = NULL;

    if (consume_token(it, ASSIGN_TOKEN)) {
        base = AST_create_node(CURRENT_TOKEN);
        PARSER_DO_OR_THROW(!base, NULL, "Can't create a base for a declaration value!");

        forward_token(it, 1);
        switch (CURRENT_TOKEN->t_type) {
            case OPEN_BLOCK_TOKEN: {
                long long act_size = 0;
                forward_token(it, 1);
                while (CURRENT_TOKEN && CURRENT_TOKEN->t_type != CLOSE_BLOCK_TOKEN) {
                    if (CURRENT_TOKEN->t_type == COMMA_TOKEN) {
                        forward_token(it, 1);
                        continue;
                    }

                    ast_node_t* elem = cpl_parse_expression(it, ctx, smt, 1);
                    PARSER_DO_OR_THROW(!elem, base, "Error during parsing of the array's initial element!");
                    AST_add_node(base, elem);
                    const_length = MAX(const_length, act_size++);
                }

                if (CURRENT_TOKEN) forward_token(it, 1);
                break;
            }
            case STRING_VALUE_TOKEN: {
                ast_node_t* elem = AST_create_node(CURRENT_TOKEN);
                PARSER_DO_OR_THROW(!elem, base, "Error during parsing of the array's initial element!");
                AST_add_node(base, elem);
                const_length = MAX(const_length, CURRENT_TOKEN->body->len(CURRENT_TOKEN->body) + 1);
                break;
            }
            default: {
                ast_node_t* value_node = cpl_parse_expression(it, ctx, smt, 1);
                PARSER_DO_OR_THROW(!value_node, base, "Error during parsing of a declaration statement!");
                AST_add_node(base, value_node);
                break;
            }
        }
    }

    if (carry) *((long long*)carry) = const_length;
    return base;
}
