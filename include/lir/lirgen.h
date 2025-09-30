#ifndef IRGEN_H_
#define IRGEN_H_

#include <symtab/symtab.h>
#include <hir/hir.h>
#include <lir/lir.h>

typedef struct lir_gen {
    int (*generate)(hir_ctx_t*, lir_ctx_t*, sym_table_t*);
} lir_gen_t;

int LIR_generate(hir_ctx_t* hctx, lir_gen_t* g, lir_ctx_t* ctx, sym_table_t* smt);

#endif