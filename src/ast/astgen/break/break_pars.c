#include <ast/astgen/astgen.h>

DEFINE_PARSER(cpl_parse_break, {
    ast_node_t* base = AST_create_node(CURRENT_TOKEN);
    PARSER_ASSERT(!base, NULL, "Can't create a base for the 'break' statement!");
    PARSER_ASSERT(!consume_token(it, DELIMITER_TOKEN), base, "Delimiter token wasn't found!");
    forward_token(it, 1);
    return base;
})
