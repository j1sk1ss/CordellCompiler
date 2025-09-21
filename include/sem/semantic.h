#ifndef SEMANTIC_H_
#define SEMANTIC_H_

#include <std/mm.h>
#include <std/str.h>
#include <std/vars.h>
#include <std/math.h>
#include <std/logg.h>
#include <prep/token.h>
#include <ast/syntax.h>
#include <limits.h>

/* heapper.c */
int SMT_check_heap_usage(syntax_ctx_t* sctx);

/* ownership.c */
int SMT_check_ownership(syntax_ctx_t* sctx);

/* rettype.c */
int SMT_check_rettype(syntax_ctx_t* sctx);

/* ro.c */
int SMT_check_ro(syntax_ctx_t* sctx);

/* size.c */
int SMT_check_sizes(syntax_ctx_t* sctx);

/* bitness.c */
int SMT_check_bitness(syntax_ctx_t* sctx);

/* semantic.c */
int SMT_check(syntax_ctx_t* sctx);

#endif