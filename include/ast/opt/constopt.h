#ifndef CONSTOPT_H_
#define CONSTOPT_H_

#include <std/mm.h>
#include <std/str.h>
#include <std/stack.h>
#include <prep/token.h>
#include <ast/syntax.h>
#include <symtab/symtab.h>

int OPT_constfold(syntax_ctx_t* ctx);

#endif