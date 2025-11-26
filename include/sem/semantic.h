#ifndef SEMANTIC_H_
#define SEMANTIC_H_

#include <std/list.h>
#include <symtab/symtab.h>
#include <ast/ast.h>
#include <ast/astgen.h>
#include <sem/ast/ast_walker.h>
#include <sem/ast/ast_visitors.h>

int SEM_perform_ast_check(ast_ctx_t* actx, sym_table_t* smt);

#endif