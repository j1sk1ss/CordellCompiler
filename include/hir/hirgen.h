#ifndef HIRGEN_H_
#define HIRGEN_H_

#include <ast/ast.h>
#include <ast/astgen.h>
#include <hir/hir.h>
#include <hir/hirgens/hirgens.h>

/*
Generate the HIR.
Params:
    - `sctx` - Input AST.
    - `hctx` - HIR context.
    - `smt` - Symtable.

Return 1 or 0 that depends on success or failure.
*/
int HIR_generate(ast_ctx_t* sctx, hir_ctx_t* hctx, sym_table_t* smt);

/*
Grab the next hir block from the HIR. 
Params:
    - `c` - Current HIR block.
    - `exit` - The exit HIR statement. 
               If this function reaches the exit statement, it will return NULL.
    - `skip` - The number of blocks that must be skipped by function.

Return the next HIR block.
*/
hir_block_t* HIR_get_next(hir_block_t* c, hir_block_t* exit, int skip);

#endif