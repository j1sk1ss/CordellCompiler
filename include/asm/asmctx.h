#ifndef ASMCTX_H_
#define ASMCTX_H_

#include <stdio.h>
#include <lir/lir.h>

typedef struct {
    int (*declarator)(lir_ctx_t*, FILE*);
    int (*generator)(lir_ctx_t*, FILE*);
} asm_gen_t;

#endif