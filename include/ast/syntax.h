#ifndef SYNTAX_H_
#define SYNTAX_H_

#include <ast/ast.h>
#include <prep/token.h>
#include <ast/synctx.h>

int STX_var_lookup(ast_node_t* node, syntax_ctx_t* ctx);
int STX_var_update(ast_node_t* node, syntax_ctx_t* ctx, const char* name, int size, token_flags_t* flags);
int STX_create(token_t* head, syntax_ctx_t* ctx);

#endif