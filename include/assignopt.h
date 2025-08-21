#ifndef ASSIGNOPT_H_
#define ASSIGNOPT_H_

#include "mm.h"
#include "str.h"
#include "stack.h"
#include "token.h"
#include "syntax.h"
#include "varmem.h"

/*
Force assign delete all local constants and inline them.
Optimization gives opportunity to reduce stack usage.
*/
int OPT_force_assign(syntax_ctx_t* ctx);

#endif