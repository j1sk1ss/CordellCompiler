#ifndef X86_64_IRGEN_H_
#define X86_64_IRGEN_H_

#include <config.h>
#include <hir/hir.h>
#include <hir/hir_types.h>
#include <lir/lir.h>
#include <lir/lirgen.h>
#include <lir/lir_types.h>
#include <symtab/symtab.h>
#include <std/map.h>
#include <std/qsort.h>
#include <std/stack.h>
#include <std/stackmap.h>

/*
Convert the HIR subject to a LIR subject.
Note: This can look as a wrong structure for a target architecture,
      but it will be fixed in the 'planning' part.
Params:
    - `subj` - HIR subject to convert.

Returns a LIR variable. Otherwise will return 0.
*/
lir_subject_t* LIR_convert_hs_to_ls(hir_subject_t* subj);

/*
Generate a LIR code from a HIR code. Will traverse blocks in functions.
Note: It will produce a HLIR form of a code, which means it isn't ready
      for the final ASM generation. To do this, you will need to obtain
      a LLIR form!
Params:
    - `cctx` - CFG context.
    - `ctx` - LIR context.
    - `smt` - Symtable.

Returns 1 if succeeds.
*/
int LIR_generate_block(cfg_ctx_t* cctx, lir_ctx_t* ctx, sym_table_t* smt);

#endif