#ifndef SYNTAX_H_
#define SYNTAX_H_

#include "ast.h"
#include "regs.h"
#include "arrmem.h"
#include "varmem.h"
#include "token.h"
#include "dict.h"
#include "vars.h"
#include "str.h"

#define MAX_SCOPE_DEPTH 1024

typedef struct {
    int data[MAX_SCOPE_DEPTH];
    int top;
} scope_stack_t;

static inline void scope_push(scope_stack_t* st, int id) {
    st->data[++st->top] = id;
}

static inline void scope_pop(scope_stack_t* st) {
    if (st->top >= 0) st->top--;
}

static inline int scope_top(scope_stack_t* st) {
    return (st->top >= 0) ? st->data[st->top] : -1;
}

typedef struct {
    const char*   scope;
    ast_node_t*   r;
    varmem_ctx_t* vars;
    arrmem_ctx_t* arrs;
} syntax_ctx_t;

syntax_ctx_t* STX_create_ctx();
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