#ifndef X86_64_ASM_GENERATOR_H_
#define X86_64_ASM_GENERATOR_H_

#include <ir/ir.h>
#include <ir/irctx.h>
#include <ast/ast.h>
#include <ast/synctx.h>

#define iprintf(out, fmt, ...) fprintf(out, fmt, ##__VA_ARGS__)

int x86_64_generate_asm(ir_block_t* h, FILE* output);
int x86_64_generate_data(ast_node_t* node, FILE* output);

#endif