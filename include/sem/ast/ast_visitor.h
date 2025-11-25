#ifndef AST_VISITOR_H_
#define AST_VISITOR_H_

#include <std/mm.h>
#include <std/str.h>
#include <prep/token_types.h>
#include <ast/ast.h>
#include <ast/astgen.h>

typedef struct {
    token_type_t trg;
    int          (*perform)(ast_node_t*);
} ast_visitor_t;

ast_visitor_t* ASTVIS_create_visitor(token_type_t trg, int (*perform)(ast_node_t*));
int ASTVIS_unload_visitor(ast_visitor_t* v);

#endif