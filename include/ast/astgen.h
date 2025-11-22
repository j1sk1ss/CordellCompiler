#ifndef ASTGEN_H_
#define ASTGEN_H_

#include <symtab/symtab.h>
#include <prep/token_types.h>
#include <ast/ast.h>
#include <ast/astgens/astgens.h>
#include <std/list.h>

/*
Move iterator with N-steps.
Params:
- it - Current iterator.
- steps - Steps count.

Return 0 if we reach end, 1 if still in list.
*/
static inline int forward_token(list_iter_t* it, int steps) {
    while (steps-- > 0) {
        if (list_iter_next_top(it)) list_iter_next(it);
        else return 0;
    }

    return 1;
}

/*
Generate AST from list of tokens.
Params:
- tkn - List of tokens.
- ctx - AST ctx.
- smt - Symtable.

Return 1 if success, otherwise 0.
*/
int AST_parse_tokens(list_t* tkn, ast_ctx_t* ctx, sym_table_t* smt);

#endif