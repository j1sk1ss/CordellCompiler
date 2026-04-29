#ifndef AST_DUMP_H_
#define AST_DUMP_H_

#include <stdio.h>
#include <string.h>
#include <ast/ast.h>
#include <prep/token.h>
#include <prep/token_types.h>

const char* DUMP_format_token(token_t* t);
int DUMP_format_astctx(ast_ctx_t* ctx, FILE* output);

#endif
