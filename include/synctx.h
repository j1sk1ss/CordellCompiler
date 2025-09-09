#ifndef SYNCTX_H_
#define SYNCTX_H_

#include "ast.h"
#include "stack.h"
#include "token.h"
#include "vartb.h"
#include "arrtb.h"

typedef struct {
    int           s_id;
    scope_stack_t stack;
} scope_info_t;

typedef struct syntax_ctx {
    ast_node_t*   r;
    varmem_ctx_t* vars;
    arrmem_ctx_t* arrs;
    scope_info_t  scopes;
} syntax_ctx_t;

typedef struct parser {
    ast_node_t* (*block)(token_t**, syntax_ctx_t*, token_type_t, struct parser*);
    ast_node_t* (*switchstmt)(token_t**, syntax_ctx_t*, struct parser*);
    ast_node_t* (*condop)(token_t**, syntax_ctx_t*, struct parser*);
    ast_node_t* (*arraydecl)(token_t**, syntax_ctx_t*, struct parser*);
    ast_node_t* (*vardecl)(token_t**, syntax_ctx_t*, struct parser*);
    ast_node_t* (*extrn)(token_t**, syntax_ctx_t*, struct parser*);
    ast_node_t* (*rexit)(token_t**, syntax_ctx_t*, struct parser*);
    ast_node_t* (*funccall)(token_t**, syntax_ctx_t*, struct parser*);
    ast_node_t* (*function)(token_t**, syntax_ctx_t*, struct parser*);
    ast_node_t* (*import)(token_t**, syntax_ctx_t*, struct parser*);
    ast_node_t* (*expr)(token_t**, syntax_ctx_t*, struct parser*);
    ast_node_t* (*scope)(token_t**, syntax_ctx_t*, struct parser*);
    ast_node_t* (*asmer)(token_t**, syntax_ctx_t*, struct parser*);
    ast_node_t* (*start)(token_t**, syntax_ctx_t*, struct parser*);
    ast_node_t* (*syscall)(token_t**, syntax_ctx_t*, struct parser*);
} parser_t;

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
int STX_var_update(ast_node_t* node, syntax_ctx_t* ctx, const char* name, int size, char ro, char glob);

#endif