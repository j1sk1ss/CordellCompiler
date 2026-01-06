#ifndef AST_WALKER_H_
#define AST_WALKER_H_

#include <symtab/symtab.h>
#include <std/list.h>
#include <ast/ast.h>
#include <ast/astgen.h>
#include <sem/ast/ast_visitor.h>

typedef enum {
    ATTENTION_UNKNOWN_LEVEL, /* Unknown attention level. Will fire a warning only with all warns enabled  */
    ATTENTION_LOW_LEVEL,     /* Low level of an attantion. Will fire a warning with a low level enabled   */
    ATTENTION_MEDIUM_LEVEL,  /* Mid level of an attantion. Will fire a warning with a mid level enabled   */
    ATTENTION_HIGH_LEVEL,    /* High level of an attantion. Will fire a warning with a high level enabled */
    ATTENTION_BLOCK_LEVEL    /* Will block code compilation if it fires                                   */
} attention_level_t;

typedef enum {
    EXPRESSION_NODE  = 1 << 0,
    ASSIGN_NODE      = 1 << 1,
    DECLARATION_NODE = 1 << 2,
    FUNCTION_NODE    = 1 << 3,
    CALL_NODE        = 1 << 4,
    START_NODE       = 1 << 5,
    DEF_ARRAY_NODE   = 1 << 6,
    IF_NODE          = 1 << 7,
    WHILE_NODE       = 1 << 8,
    TERM_NODE        = 1 << 9,
    UNKNOWN_NODE     = 1 << 10,
} ast_node_type_t;

typedef struct {
    attention_level_t l;
    ast_visitor_t*    w;
} ast_sem_handler_t;

typedef struct {
    list_t       visitors;
    sym_table_t* smt;
} ast_walker_t;

/*
Register a new one walker to the walker.
Params:
    - `trg` - Target node (-s) type. 
              Note: To add a support for a few nodes, use
                    a bit 'or' operation.
                    For instance: ASSIGN_NODE | CALL_NODE
    - `perform` - AST walker handler. Will invoke this 
                  function when encounter with the 'trg'.
    - `ctx` - Walker context.
    - `l` - Attention level. Check attention_level_t to see details.

Returns 1 if succeeds.
*/
int ASTWLK_register_visitor(unsigned int trg, int (*perform)(ast_node_t*, sym_table_t*), ast_walker_t* ctx, attention_level_t l);

/*
Init a new one walker context.
Params:
    - `ctx` - Walker context.
    - `smt` - Symtable.

Returns 1 if succeeds.
*/
int ASTWLK_init_ctx(ast_walker_t* ctx, sym_table_t* smt);

/*
Performs the main walker routeen. 
Params:
    - `actx` - AST context.
    - `ctx` - Walker context.

Returns 1 if succeeds.
*/
int ASTWLK_walk(ast_ctx_t* actx, ast_walker_t* ctx);

/*
Unload the context.
Params:
    - `ctx` - Walker context.

Returns 1 if succeeds.
*/
int ASTWLK_unload_ctx(ast_walker_t* ctx);

#endif
