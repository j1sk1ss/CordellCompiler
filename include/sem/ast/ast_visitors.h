#ifndef AST_VISITORS_H_
#define AST_VISITORS_H_

#include <std/logg.h>
#include <prep/token_types.h>
#include <ast/ast.h>
#include <ast/astgen.h>

int ASTWLKR_ro_assign(ast_node_t* nd, sym_table_t* smt);
int ASTWLKR_rtype_assign(ast_node_t* nd, sym_table_t* smt);
int ASTWLKR_not_init(ast_node_t* nd, sym_table_t* smt);
int ASTWLKR_no_return(ast_node_t* nd, sym_table_t* smt);
int ASTWLKR_no_exit(ast_node_t* nd, sym_table_t* smt);

#endif