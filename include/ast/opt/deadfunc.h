#ifndef DEADFUNC_H_
#define DEADFUNC_H_

#include <std/mm.h>
#include <std/str.h>
#include <std/stack.h>
#include <prep/token_types.h>
#include <ast/astgen.h>

#define DCTX_AST_MAX 100
typedef struct {
    ast_ctx_t* ctx[DCTX_AST_MAX];
    char          size;
} deadfunc_ctx_t;

/*
Add AST for deadfunc cleanup.
Note: Deadfunc - functions that never used.
*/
int OPT_deadfunc_add(ast_ctx_t* ctx, deadfunc_ctx_t* dctx);

/*
Clear all dead functions in linked to dctx AST.
Note: Iterates through all AST untill it delete some function.
*/
int OPT_deadfunc_clear(deadfunc_ctx_t* dctx);

#endif