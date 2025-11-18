#ifndef SYNTAX_H_
#define SYNTAX_H_

#include <std/list.h>
#include <symtab/symtab.h>
#include <prep/token_types.h>
#include <ast/ast.h>
#include <ast/synctx.h>
#include <ast/parsers/parser.h>

/*
Create AST from list of tokens.
Params:
- tkn - List of tokens.
- ctx - AST context.
- smt - Symtables.

Return 1 if AST generation complete without errors.
*/
int AST_parse_tokens(list_t* tkn, ast_ctx_t* ctx, sym_table_t* smt);

#endif