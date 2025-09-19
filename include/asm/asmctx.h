#ifndef ASMCTX_H_
#define ASMCTX_H_

#include <stdio.h>
#include <ast/ast.h>
#include <ir/ir.h>

typedef struct {
    int (*declarator)(ast_node_t*, FILE*);
    int (*generator)(ir_block_t*, FILE*);
} asm_gen_t;

#endif