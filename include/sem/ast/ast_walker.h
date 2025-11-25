#ifndef AST_WALKER_H_
#define AST_WALKER_H_

#include <std/list.h>
#include <ast/ast.h>
#include <ast/astgen.h>
#include <sem/ast/ast_visitor.h>

typedef enum {
    EXPRESSION_NODE,
    ASSIGN_NODE,
    DECLARATION_NODE,
    UNKNOWN_NODE
} ast_node_type_t;

typedef struct {
    list_t visitors;
} ast_walker_t;

int ASTWLK_register_visitor(token_type_t trg, int (*perform)(ast_node_t*), ast_walker_t* ctx);
int ASTWLK_init_ctx(ast_walker_t* ctx);
int ASTWLK_walk(ast_ctx_t* actx, ast_walker_t* ctx);
int ASTWLK_unload_ctx(ast_walker_t* ctx);

#endif