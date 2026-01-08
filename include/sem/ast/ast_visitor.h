#ifndef AST_VISITOR_H_
#define AST_VISITOR_H_

#include <std/mm.h>
#include <std/str.h>
#include <prep/token_types.h>
#include <ast/ast.h>
#include <ast/astgen.h>

typedef struct {
    unsigned int trg;
    int          (*perform)(ast_node_t*, sym_table_t*);
} ast_visitor_t;

/*
Create a new visitor for an AST node.
Params:
    - `trg` - Target nodes.
    - `perform` - Function handler.

Returns a new one visitor.
*/
ast_visitor_t* ASTVIS_create_visitor(unsigned int trg, int (*perform)(ast_node_t*, sym_table_t*));

/*
Unload a visitor.
Params:
    - `v` - Visitor.

Returns 1 if succeeds.
*/
int ASTVIS_unload_visitor(ast_visitor_t* v);

#endif