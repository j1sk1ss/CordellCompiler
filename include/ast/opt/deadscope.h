#ifndef DEADSCOPE_H_
#define DEADSCOPE_H_

#include <ast/astgen.h>

/*
Perform dead scope optimization. Main idea here is to delete all scopes, that didn't affect on environment.
Affecting is a changing some outer-scope variable, calling some function or invoking a syscall.
Params:
    - ctx - AST

Return 1 if optimization is performed.
Return 0 if something goes wrong.
*/
int OPT_deadscope(ast_ctx_t* ctx);

#endif