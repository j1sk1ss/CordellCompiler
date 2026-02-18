#ifndef ASTGEN_H_
#define ASTGEN_H_

#include <symtab/symtab.h>
#include <prep/token_types.h>
#include <ast/ast.h>
#include <ast/astgen/astgen.h>
#include <std/list.h>

/*
Move the iterator with N-steps.
Params:
    - `it` - Current iterator.
    - `steps` - Steps count.

Return 0 if we reach the end, 1 if still in the list.
*/
static inline int forward_token(list_iter_t* it, int steps) {
    while (steps-- > 0) {
        if (list_iter_next_top(it)) list_iter_next(it);
        else return 0;
    }

    return 1;
}

/*
Move the iterator backward with N-steps.
Params:
    - `it` - Current iterator.
    - `steps` - Steps count.

Returns 0 of we reach the start, 1 is still in the list.
*/
static inline int backward_token(list_iter_t* it, int steps) {
    while (steps-- > 0) {
        if (list_iter_prev_top(it)) list_iter_prev(it);
        else return 0;
    }

    return 1;
}

/*
Check if the next token is consumed.
Note: Before the check will move a token iterator first.
Params:
    - `it` - Iterator.
    - `t` - Target token's type.

Returns 1 if a token is consumed. Otherwise will return 0.
*/
static inline int consume_token(list_iter_t* it, token_type_t t) {
    if (!forward_token(it, 1)) return 0;
    return ((token_t*)list_iter_current(it))->t_type == t;
}

/*
Init an AST context.
Params:
    - `ctx` - AST context.
    - `fentry` - Default entry name for the architecture.
                 Note: By default can be NULL.

Returns 1 if succeeds.
*/
int AST_init_ctx(ast_ctx_t* ctx, const char* fentry);

/*
Generate an AST tree from a list of tokens.
Params:
    - `tkn` - List of tokens.
    - `ctx` - AST ctx.
    - `smt` - Symtable.

Returns 1 if succeeds, otherwise will return 0.
*/
int AST_parse_tokens(list_t* tkn, ast_ctx_t* ctx, sym_table_t* smt);

/*
Unload an AST context.
Params:
    - `ctx` - AST context.

Returns 1 if succeeds.
*/
int AST_unload_ctx(ast_ctx_t* ctx);

#endif