#ifndef DEADOPT_H_
#define DEADOPT_H_

#include <std/mm.h>
#include <std/str.h>
#include <ast/ast.h>
#include <ast/syntax.h>

int OPT_deadcode(syntax_ctx_t* ctx);

#endif