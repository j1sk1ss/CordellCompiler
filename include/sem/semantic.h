#ifndef SEMANTIC_H_
#define SEMANTIC_H_

#include <std/mm.h>
#include <std/str.h>
#include <prep/token_types.h>
#include <std/math.h>
#include <std/logg.h>
#include <symtab/symtab.h>
#include <ast/astgen.h>
#include <limits.h>

/* heapper.c */
int SMT_check_heap_usage(ast_ctx_t* sctx);

/* ownership.c */
int SMT_check_ownership(ast_ctx_t* sctx);

/* rettype.c */
int SMT_check_rettype(ast_ctx_t* sctx);

/* ro.c */
int SMT_check_ro(ast_ctx_t* sctx);

/* size.c */
int SMT_check_sizes(ast_ctx_t* sctx);

/* bitness.c */
int SMT_check_bitness(ast_ctx_t* sctx);

/* semantic.c */
int SMT_check(ast_ctx_t* sctx);

#endif