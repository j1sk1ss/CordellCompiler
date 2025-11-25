#ifndef AST_VISITORS_H_
#define AST_VISITORS_H_

#include <std/logg.h>
#include <ast/ast.h>
#include <ast/astgen.h>

int ASTWLKR_ro_assign(ast_node_t* nd);
int ASTWLKR_rtype_assign(ast_node_t* nd);

#endif