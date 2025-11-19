#ifndef ASTGEN_H_
#define ASTGEN_H_

#include <symtab/symtab.h>
#include <prep/token_types.h>
#include <ast/ast.h>
#include <ast/astgens/astgens.h>
#include <std/list.h>

static inline int forward_token(list_iter_t* it, int steps) {
    while (steps-- > 0) {
        if (list_iter_next_top(it)) list_iter_next(it);
        else return 0;
    }

    return 1;
}

int AST_parse_tokens(list_t* tkn, ast_ctx_t* ctx, sym_table_t* smt);

#endif