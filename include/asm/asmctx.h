#ifndef ASMCTX_H_
#define ASMCTX_H_

#include <stdio.h>
#include <ast/ast.h>
#include <lir/lir.h>

typedef struct {
    int (*declarator)(ast_node_t*, FILE*);
    int (*generator)(lir_block_t*, FILE*);
} asm_gen_t;

#endif