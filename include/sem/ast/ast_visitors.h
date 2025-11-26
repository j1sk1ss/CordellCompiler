#ifndef AST_VISITORS_H_
#define AST_VISITORS_H_

#include <limits.h>
#include <std/logg.h>
#include <prep/token_types.h>
#include <ast/ast.h>
#include <ast/astgen.h>

#define AST_VISITOR_ARGS ast_node_t* nd, sym_table_t* smt

int ASTWLKR_ro_assign(AST_VISITOR_ARGS);
int ASTWLKR_rtype_assign(AST_VISITOR_ARGS);
int ASTWLKR_not_init(AST_VISITOR_ARGS);
int ASTWLKR_illegal_declaration(AST_VISITOR_ARGS);
int ASTWLKR_no_return(AST_VISITOR_ARGS);
int ASTWLKR_no_exit(AST_VISITOR_ARGS);
int ASTWLKR_not_enough_args(AST_VISITOR_ARGS);
int ASTWLKR_illegal_array_access(AST_VISITOR_ARGS);

#endif