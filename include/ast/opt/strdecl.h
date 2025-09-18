#ifndef STROPT_H_
#define STROPT_H_

#include <std/mm.h>
#include <std/str.h>
#include <std/vars.h>
#include <prep/dict.h>
#include <prep/token.h>
#include <ast/syntax.h>

/*
Move strings to .data section
Note: Will move all signle strings to .data
Note 2: Will move all strings assigned to ptr to .data
*/
int OPT_strpack(syntax_ctx_t* ctx);

#endif