#ifndef STROPT_H_
#define STROPT_H_

#include "mm.h"
#include "str.h"
#include "dict.h"
#include "vars.h"
#include "token.h"
#include "syntax.h"

/*
Move strings to .data section
Note: Will move all signle strings to .data
Note 2: Will move all strings assigned to ptr to .data
*/
int OPT_strpack(syntax_ctx_t* ctx);

#endif