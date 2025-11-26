#ifndef AST_WALKER_H_
#define AST_WALKER_H_

#include <symtab/symtab.h>
#include <std/list.h>
#include <ast/ast.h>
#include <ast/astgen.h>
#include <sem/ast/ast_visitor.h>

typedef enum {
    EXPRESSION_NODE  = 1 << 0,
    ASSIGN_NODE      = 1 << 1,
    DECLARATION_NODE = 1 << 2,
    FUNCTION_NODE    = 1 << 3,
    CALL_NODE        = 1 << 4,
    START_NODE       = 1 << 5,
    DEF_ARRAY_NODE   = 1 << 6,
    UNKNOWN_NODE     = 1 << 7,
} ast_node_type_t;

typedef struct {
    list_t       visitors;
    sym_table_t* smt;
} ast_walker_t;

int ASTWLK_register_visitor(token_type_t trg, int (*perform)(ast_node_t*), ast_walker_t* ctx);
int ASTWLK_init_ctx(ast_walker_t* ctx, sym_table_t* smt);
int ASTWLK_walk(ast_ctx_t* actx, ast_walker_t* ctx);
int ASTWLK_unload_ctx(ast_walker_t* ctx);

#endif