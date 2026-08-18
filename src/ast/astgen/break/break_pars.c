#include <ast/astgen/astgen.h>

ast_node_t* cpl_parse_break(PARSER_ARGS) {
    PARSER_ARGS_USE;
    SAVE_TOKEN_POINT;
    ast_node_t* base = AST_create_node(CURRENT_TOKEN);
    PARSER_DO_OR_THROW(!base, NULL, "Can't create a base for the 'break' statement!");
    PARSER_DO_OR_THROW(!consume_token(it, DELIMITER_TOKEN), base, "Delimiter token wasn't found!");
    forward_token(it, 1);
    return base;
}
