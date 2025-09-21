#ifndef SYNTAX_H_
#define SYNTAX_H_

#include <ast/ast.h>
#include <prep/token.h>
#include <ast/synctx.h>
#include <ast/parsers/parser.h>
#include <symtab/symtab.h>

int STX_create(token_t* head, syntax_ctx_t* ctx, sym_table_t* smt);

#endif