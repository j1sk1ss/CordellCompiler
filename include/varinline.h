#ifndef VARINLINE_H_
#define VARINLINE_H_

#include "mm.h"
#include "str.h"
#include "token.h"
#include "syntax.h"

/*
Force assign deletes all local constants and inline them.
Optimization reduces stack usage via declaration elemination.
*/
int OPT_varinline(syntax_ctx_t* ctx);

#endif