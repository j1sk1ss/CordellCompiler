#ifndef SYNTAX_H_
#define SYNTAX_H_

#include <ast/ast.h>
#include <prep/token.h>
#include <ast/synctx.h>

/*
Generate AST from tokens list.
Params: 
- head - Tokens list head.
- ctx - Parser context.

Return 0 if somehing goes wrong.
Return 1 if markup success.
*/
int STX_create(token_t* head, syntax_ctx_t* ctx, parser_t* p);

#endif