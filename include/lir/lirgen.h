#ifndef IRGEN_H_
#define IRGEN_H_

#include <symtab/symtab.h>
#include <hir/hir.h>
#include <hir/cfg.h>
#include <lir/lir.h>
#include <lir/lirgens/lirgens.h>

/*
Generate the LIR.
Params:
    - cctx - Input CFG with the HIR.
    - ctx - LIR context.
    - smt - Symtable.

Return 1 or 0 that depends on success or failure.
*/
int LIR_generate(cfg_ctx_t* cctx, lir_ctx_t* ctx, sym_table_t* smt);

/*
Grab the next lit block from the LIR. 
Params:
    - c - Current LIR block.
    - exit - The exit LIR statement. 
             If this function reaches the exit statement, it will returns NULL.
    - skip - The number of blocks that must be skipped by function.

Return the next LIR block.
*/
lir_block_t* LIR_get_next(lir_block_t* c, lir_block_t* exit, int skip);

#endif