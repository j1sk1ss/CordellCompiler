#ifndef AST_VISITOR_H_
#define AST_VISITOR_H_

#include <std/mm.h>
#include <std/str.h>
#include <prep/token_types.h>
#include <ast/ast.h>
#include <ast/astgen.h>
#include <sem/ast/ast_data.h>

typedef struct {
    unsigned int trg;
    int          (*perform)(AST_VISITOR_ARGS);
} ast_visitor_t;

/*
Create a new visitor for an AST node.
Params:
    - `trg` - Target nodes.
    - `perform` - Function handler.

Returns a new one visitor.
*/
ast_visitor_t* ASTVIS_create_visitor(unsigned int trg, int (*perform)(AST_VISITOR_ARGS));

/*
Unload a visitor.
Params:
    - `v` - Visitor.

Returns 1 if succeeds.
*/
int ASTVIS_unload_visitor(ast_visitor_t* v);

#endif