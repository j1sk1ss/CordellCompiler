#ifndef SYNCTX_H_
#define SYNCTX_H_

#include <ast/ast.h>
#include <std/stack.h>
#include <prep/token.h>
#include <symtab/symtab.h>

typedef struct {
    int           s_id;
    scope_stack_t stack;
} scope_info_t;

typedef struct {
    ast_node_t*  r;
    sym_table_t  symtb;
    scope_info_t scopes;
} syntax_ctx_t;

static inline int forward_token(token_t** tkn, int steps) {
    while (steps-- > 0) {
        if ((*tkn)->next) *tkn = (*tkn)->next;
        else return 0;
    }

    return 1;
}

syntax_ctx_t* STX_create_ctx();
int STX_destroy_ctx(syntax_ctx_t* ctx);
int STX_var_lookup(ast_node_t* node, syntax_ctx_t* ctx);
int STX_var_update(ast_node_t* node, syntax_ctx_t* ctx, const char* name, int size, token_flags_t* flags);

#endif