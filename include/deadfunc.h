#ifndef DEADFUNC_H_
#define DEADFUNC_H_

#include "mm.h"
#include "str.h"
#include "stack.h"
#include "token.h"
#include "syntax.h"

typedef struct {
    syntax_ctx_t* ctx[100];
    char size;
} deadfunc_ctx_t;

int OPT_deadfunc_add(syntax_ctx_t* ctx, deadfunc_ctx_t* dctx);
int OPT_deadfunc_clear(deadfunc_ctx_t* dctx);

#endif