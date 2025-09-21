#ifndef OFFSETOPT_H_
#define OFFSETOPT_H_

#include <std/mm.h>
#include <std/str.h>
#include <std/stack.h>
#include <std/stackmap.h>
#include <prep/token.h>
#include <ast/syntax.h>
#include <symtab/symtab.h>

int OPT_offrecalc(syntax_ctx_t* ctx);

#endif