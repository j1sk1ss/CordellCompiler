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
int SMT_check_heap_usage(ast_node_t* node);

/* ownership.c */
int SMT_check_ownership(ast_node_t* node);

/* rettype.c */
int SMT_check_rettype(ast_node_t* node);

/* ro.c */
int SMT_check_ro(ast_node_t* node);

/* size.c */
int SMT_check_sizes(ast_node_t* node);

/* bitness.c */
int SMT_check_bitness(ast_node_t* node, sym_tables_t* smt);

/* semantic.c */
int SMT_check(ast_node_t* node, sym_tables_t* smt);

#endif