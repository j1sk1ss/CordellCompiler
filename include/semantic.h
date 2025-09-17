#ifndef SEMANTIC_H_
#define SEMANTIC_H_

#include "mm.h"
#include "str.h"
#include "vars.h"
#include "math.h"
#include "logg.h"
#include "token.h"
#include "syntax.h"
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