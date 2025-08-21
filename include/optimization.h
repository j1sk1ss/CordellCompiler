#ifndef OPTIMIZATION_H_
#define OPTIMIZATION_H_

#include "mm.h"
#include "str.h"
#include "stack.h"
#include "token.h"
#include "syntax.h"
#include "varmem.h"

#include <stdio.h>

int OPT_strpack(syntax_ctx_t* ctx);
int OPT_varuse(syntax_ctx_t* ctx);
int OPT_muldiv(syntax_ctx_t* ctx);
int OPT_offrecalc(syntax_ctx_t* ctx);
int stmt_optimization(syntax_ctx_t* ctx);

int funcopt_add_ast(syntax_ctx_t* ctx);
int func_optimization();

#endif