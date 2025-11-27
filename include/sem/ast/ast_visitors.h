#ifndef AST_VISITORS_H_
#define AST_VISITORS_H_

#include <stdio.h>
#include <limits.h>
#include <std/str.h>
#include <prep/token_types.h>
#include <ast/ast.h>
#include <ast/astgen.h>

#define AST_VISITOR_ARGS ast_node_t* nd, sym_table_t* smt
#define SEMANTIC_ERROR(message, ...)   fprintf(stdout, "[ERROR]  " message "\n", ##__VA_ARGS__)
#define SEMANTIC_WARNING(message, ...) fprintf(stdout, "[WARNING]" message "\n", ##__VA_ARGS__)
#define SEMANTIC_INFO(message, ...)    fprintf(stdout, "[INFO]   " message "\n", ##__VA_ARGS__)

int ASTWLKR_ro_assign(AST_VISITOR_ARGS);
int ASTWLKR_rtype_assign(AST_VISITOR_ARGS);
int ASTWLKR_not_init(AST_VISITOR_ARGS);
int ASTWLKR_illegal_declaration(AST_VISITOR_ARGS);
int ASTWLKR_no_return(AST_VISITOR_ARGS);
int ASTWLKR_no_exit(AST_VISITOR_ARGS);
int ASTWLKR_not_enough_args(AST_VISITOR_ARGS);
int ASTWLKR_wrong_arg_type(AST_VISITOR_ARGS);
int ASTWLKR_unused_rtype(AST_VISITOR_ARGS);
int ASTWLKR_illegal_array_access(AST_VISITOR_ARGS);
int ASTWLKR_valid_function_name(AST_VISITOR_ARGS);

#endif