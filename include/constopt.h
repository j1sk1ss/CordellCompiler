#ifndef CONSTOPT_H_
#define CONSTOPT_H_

#include "mm.h"
#include "str.h"
#include "stack.h"
#include "token.h"
#include "syntax.h"
#include "varmem.h"

int OPT_constfold(syntax_ctx_t* ctx);

#endif