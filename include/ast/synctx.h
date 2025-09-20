#ifndef SYNCTX_H_
#define SYNCTX_H_

#include <std/stack.h>
#include <prep/token.h>
#include <ast/ast.h>
#include <ast/tables/vartb.h>
#include <ast/tables/arrtb.h>
#include <ast/tables/functb.h>

typedef struct {
    int           s_id;
    scope_stack_t stack;
} scope_info_t;

typedef struct {
    vartab_ctx_t*  vars;
    arrtab_ctx_t*  arrs;
    functab_ctx_t* funcs;
} sym_tables_t;

typedef struct syntax_ctx {
    ast_node_t*  r;
    sym_tables_t symtb;
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