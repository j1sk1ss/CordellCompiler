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
    ast_node_t*   (*block)(token_t**, struct syntax_ctx*, token_type_t);
    ast_node_t*   (*switchstmt)(token_t**, struct syntax_ctx*);
    ast_node_t*   (*condop)(token_t**, struct syntax_ctx*);
    ast_node_t*   (*arraydecl)(token_t**, struct syntax_ctx*);
    ast_node_t*   (*vardecl)(token_t**, struct syntax_ctx*);
    ast_node_t*   (*rexit)(token_t**, struct syntax_ctx*);
    ast_node_t*   (*funccall)(token_t**, struct syntax_ctx*);
    ast_node_t*   (*function)(token_t**, struct syntax_ctx*);
    ast_node_t*   (*import)(token_t**, struct syntax_ctx*);
    ast_node_t*   (*expr)(token_t**, struct syntax_ctx*);
    ast_node_t*   (*scope)(token_t**, struct syntax_ctx*);
    ast_node_t*   (*start)(token_t**, struct syntax_ctx*);
    ast_node_t*   (*syscall)(token_t**, struct syntax_ctx*);
} syntax_ctx_t;

static inline int forward_token(token_t** tkn, int steps) {
    while (steps-- > 0) {
        if ((*tkn)->next) *tkn = (*tkn)->next;
        else return 0;
    }

    return 1;
}

/* Save variable to ctx varmem list */
static inline int var_update(ast_node_t* node, syntax_ctx_t* ctx, const char* name, int size, char ro, char glob) {
    if (!node) return 0;
    node->info.size   = size;
    node->info.offset = VRT_add_info(name, size, ro, glob, scope_id_top(&ctx->scopes.stack), ctx->vars);
    return 1;
}

/* Lookup variable from vartable in ctx varmem list, save offset and size */
static inline int var_lookup(ast_node_t* node, syntax_ctx_t* ctx) {
    if (!node) return 0;
    var_lookup(node->sibling, ctx);
    var_lookup(node->child, ctx);

    if (!node->token) return 0;
    variable_info_t varinfo = { .offset = -1 };
    for (int s = ctx->scopes.stack.top; s >= 0; s--) {
        int s_id = ctx->scopes.stack.data[s].id;
        if (VRT_get_info(node->token->value, s_id, &varinfo, ctx->vars)) {
            break;
        }
    }
    
    if (varinfo.offset == -1) return 0;
    node->info.offset = varinfo.offset;
    node->info.size   = varinfo.size;
    node->info.s_id   = varinfo.scope;
    return 1;
}

#endif