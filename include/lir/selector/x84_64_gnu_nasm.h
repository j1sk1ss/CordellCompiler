/* Architecture dependent module wich is linked to x86_64 GNU NASM. */
#ifndef INSTSEL_X86_64_GNU_NASM_LINUX_
#define INSTSEL_X86_64_GNU_NASM_LINUX_

#include <std/map.h>
#include <std/list.h>
#include <std/queue.h>
#include <std/stackmap.h>

#include <lir/lir.h>
#include <lir/lirgens/lirgens.h>

#include <symtab/symtab.h>
#include <prep/token_types.h>
#include <hir/cfg.h>

/*
Create a temporary virtual variable that is linked to a physical register. 
Note: Virtual variable is a copy of the existed one from the LIR.
Params:
    - `reg` - Physical register.
    - `src` - Virtual variable base.
    - `smt` - Symtable.
    - `forced_size` - Force the function to create a register with the
                      selected size regardless of the `src` size.
                      Note: If select as a negative, won't force the function.

Return the virtual variable that is linked to the physical register.
*/
lir_subject_t* x86_64_gnu_nasm_create_tmp(lir_registers_t reg, lir_subject_t* src, sym_table_t* smt, int forced_size);

/*
Checks a variable if it is a signed or not.
Params:
    - `s` - LIR subject.
    - `smt` - Symtable.

Return 1 either this is a signed variable or this isn't variable at all.
*/
int x86_64_gnu_nasm_is_sign_type(lir_subject_t* s, sym_table_t* smt);

/*
Checks a variable is SIMD by the provided variable ID. 
Note: SIMD in this context is a set of variable types such as 
      F64 (tmp/stack/glb) and F32(tmp/stack/glb).
Params:
    - `vid` - Variable ID.
    - `smt` - Symtable.

Return 1 if the variable is SIMD.
*/
int x86_64_gnu_nasm_is_simd_type(lir_subject_t* s, sym_table_t* smt);

/*
Get a mov operation for given input operands. Will return the base, if we 
won't change anything.
Note: Base by default is the 'LIR_iMOV' operation.
Note 2: If we've choosen a base operation, we will set the size
        of the second argument to the size of the first argument.
Params:
    - `a` - Destination operand.
    - `b` - Source operand.
    - `smt` - Symtable.
    - `base` - Base operation.

Returns a mov operation that is valid for given args.
*/
lir_operation_t x86_64_gnu_nasm_get_proper_mov(lir_subject_t* a, lir_subject_t* b, sym_table_t* smt, lir_operation_t base);

int x86_64_gnu_nasm_instruction_selection(cfg_ctx_t* cctx, sym_table_t* smt);
int x86_64_gnu_nasm_memory_selection(cfg_ctx_t* cctx, map_t* colors, sym_table_t* smt);
int x86_64_gnu_nasm_caller_saving(cfg_ctx_t* cctx, sym_table_t* smt);

#endif
