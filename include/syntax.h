#ifndef SYNTAX_H_
#define SYNTAX_H_

#include "ast.h"
#include "regs.h"
#include "stack.h"
#include "arrmem.h"
#include "varmem.h"
#include "token.h"
#include "dict.h"
#include "vars.h"
#include "str.h"

typedef struct {
    int           scope_id;
    scope_stack_t stack;
} scope_info_t;

typedef struct {
    ast_node_t*   r;
    varmem_ctx_t* vars;
    arrmem_ctx_t* arrs;
    scope_info_t  scope;
} syntax_ctx_t;

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
Preparing tokens list for parsing tree.
We mark every token by command / symbol ar value type.
Params:
- head - Tokens list head.

Return -1 if somehing goes wrong.
Return 1 if markup success.
*/
int MRKP_mnemonics(token_t* head);

/*
Iterate throught tokens and mark variables. 
Params:
- head - Tokens list head.

Return -1 if somehing goes wrong.
Return 1 if markup success.
*/
int MRKP_variables(token_t* head);

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