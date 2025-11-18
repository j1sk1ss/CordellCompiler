#ifndef HIRGEN_H_
#define HIRGEN_H_

#include <ast/ast.h>
#include <ast/astgen.h>
#include <hir/hir.h>
#include <hir/hirgens/hirgens.h>

int HIR_generate(ast_ctx_t* sctx, hir_ctx_t* hctx, sym_table_t* smt);

#endif