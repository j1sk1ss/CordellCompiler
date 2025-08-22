#ifndef VARINLINE_H_
#define VARINLINE_H_

#include "mm.h"
#include "str.h"
#include "token.h"
#include "syntax.h"

/*
Force assign delete all local constants and inline them.
Optimization gives opportunity to reduce stack usage.
*/
int OPT_varinline(syntax_ctx_t* ctx);

#endif