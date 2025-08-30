#ifndef SYNTAX_H_
#define SYNTAX_H_

#include "ast.h"
#include "token.h"
#include "synctx.h"

/*
Create syntax context in heap.
Return pointer to context.
*/
syntax_ctx_t* STX_create_ctx();

/*
Unload syntax context from heap.
Return 1 if unload was success.
*/
int STX_destroy_ctx(syntax_ctx_t* ctx);

/*
Generate AST from tokens list.
Params: 
- head - Tokens list head.
- ctx - Parser context.

Return 0 if somehing goes wrong.
Return 1 if markup success.
*/
int STX_create(token_t* head, syntax_ctx_t* ctx);

#endif