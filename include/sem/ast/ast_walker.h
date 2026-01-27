#ifndef AST_WALKER_H_
#define AST_WALKER_H_

#include <symtab/symtab.h>
#include <std/math.h>
#include <std/list.h>
#include <ast/ast.h>
#include <ast/astgen.h>
#include <sem/ast/ast_data.h>
#include <sem/ast/ast_visitor.h>

typedef struct {
    attention_level_t l;
    ast_visitor_t*    w;
} ast_sem_handler_t;

typedef struct {
    walker_flags_t flags;
    list_t         visitors;
    sym_table_t*   smt;
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
int ASTWLK_register_visitor(unsigned int trg, int (*perform)(AST_VISITOR_ARGS), ast_walker_t* ctx, attention_level_t l);

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
