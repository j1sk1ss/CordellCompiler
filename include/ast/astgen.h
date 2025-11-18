#ifndef ASTGEN_H_
#define ASTGEN_H_

#include <symtab/symtab.h>
#include <prep/token_types.h>
#include <ast/ast.h>
#include <std/list.h>
#include <std/stack.h>

typedef struct {
    int           s_id;
    scope_stack_t stack;
} scope_info_t;

typedef struct {
    ast_node_t*  r;
    scope_info_t scopes;
} ast_ctx_t;

static inline int forward_token(list_iter_t* it, int steps) {
    while (steps-- > 0) {
        if (list_iter_next_top(it)) list_iter_next(it);
        else return 0;
    }

    return 1;
}

#endif