/* Architecture dependent module wich is linked to x86_64 GNU NASM. */
#ifndef INSTSEL_X86_64_GNU_NASM_
#define INSTSEL_X86_64_GNU_NASM_

#include <std/map.h>
#include <std/list.h>
#include <std/stackmap.h>

#include <lir/lir.h>
#include <lir/lirgens/lirgens.h>

#include <symtab/symtab.h>
#include <prep/token_types.h>
#include <hir/cfg.h>

#define DEFAULT_TYPE_SIZE       8
#define DEFAULT_TYPE_SIZE_HALF  (DEFAULT_TYPE_SIZE) / 2
#define DEFAULT_TYPE_SIZE_QUART (DEFAULT_TYPE_SIZE_HALF) / 2
#define DEFAULT_TYPE_SIZE_ONE   (DEFAULT_TYPE_SIZE_QUART) / 2
static inline int _get_ast_type_size(token_type_t t) {
    switch (t) {
        case TMP_I64_TYPE_TOKEN: case TMP_U64_TYPE_TOKEN: case TMP_F64_TYPE_TOKEN:
        case I64_TYPE_TOKEN:     case U64_TYPE_TOKEN:     case F64_TYPE_TOKEN:     return DEFAULT_TYPE_SIZE;
        case TMP_I32_TYPE_TOKEN: case TMP_U32_TYPE_TOKEN: case TMP_F32_TYPE_TOKEN:
        case I32_TYPE_TOKEN:     case U32_TYPE_TOKEN:     case F32_TYPE_TOKEN:     return DEFAULT_TYPE_SIZE_HALF;
        case TMP_I16_TYPE_TOKEN: case TMP_U16_TYPE_TOKEN:
        case I16_TYPE_TOKEN:     case U16_TYPE_TOKEN:                              return DEFAULT_TYPE_SIZE_QUART;
        case TMP_I8_TYPE_TOKEN:  case TMP_U8_TYPE_TOKEN:
        case I8_TYPE_TOKEN:      case U8_TYPE_TOKEN:                               return DEFAULT_TYPE_SIZE_ONE;
        default: return DEFAULT_TYPE_SIZE;
    }
}

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
lir_subject_t* create_tmp(lir_registers_t reg, lir_subject_t* src, sym_table_t* smt, int forced_size);

/*
Checks a variable if it is a signed or not.
Params:
    - `s` - LIR subject.
    - `smt` - Symtable.

Return 1 either this is a signed variable or this isn't variable at all.
*/
int is_sign_type(lir_subject_t* s, sym_table_t* smt);

/*
Checks a variable is SIMD by the provided variable ID. 
Note: SIMD in this context is a set of variable types such as 
      F64 (tmp/stack/glb) and F32(tmp/stack/glb).
Params:
    - `vid` - Variable ID.
    - `smt` - Symtable.

Return 1 if the variable is SIMD.
*/
int is_simd_type(lir_subject_t* s, sym_table_t* smt);

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
lir_operation_t get_proper_mov(lir_subject_t* a, lir_subject_t* b, sym_table_t* smt, lir_operation_t base);

int x86_64_gnu_nasm_instruction_selection(cfg_ctx_t* cctx, sym_table_t* smt);
int x86_64_gnu_nasm_memory_selection(cfg_ctx_t* cctx, map_t* colors, sym_table_t* smt);
int x86_64_gnu_nasm_caller_saving(cfg_ctx_t* cctx);

#endif
