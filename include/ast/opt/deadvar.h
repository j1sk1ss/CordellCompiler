#ifndef DEADVAR_H_
#define DEADVAR_H_

#include <std/mm.h>
#include <std/str.h>
#include <std/vars.h>
#include <prep/token.h>
#include <ast/syntax.h>

int OPT_deadvar(syntax_ctx_t* ctx);

#endif