#ifndef HIRGEN_H_
#define HIRGEN_H_

#include <ast/ast.h>
#include <ast/syntax.h>
#include <hir/hir.h>
#include <hir/hirgens/hirgens.h>

int HIR_generate(hir_ctx_t* ctx);

#endif