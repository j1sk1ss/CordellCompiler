#ifndef SEMANTIC_H_
#define SEMANTIC_H_

#include "mm.h"
#include "str.h"
#include "vars.h"
#include "logg.h"
#include "token.h"
#include "syntax.h"

#include <math.h>

/*
SMT_check function iterate throught AST and check semantic.
Params:
- root - AST head.

Return 1 if semantic is true.
Return 0 if semantic is wrong.
*/
int SMT_check(ast_node_t* root);

#endif