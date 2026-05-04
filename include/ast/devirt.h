#ifndef AST_DEVIRT_H_
#define AST_DEVIRT_H_

#include <symtab/symtab.h>
#include <ast/ast.h>

int AST_resolve_calls(ast_node_t* root, sym_table_t* smt);
ast_node_t* AST_implement_template(ast_node_t* root, symbol_id_t f_id, sym_table_t* smt);
int AST_destroy_template_implementation(ast_node_t* node);

#endif
