#ifndef INSTPLAN_X86_64_GNU_NASM_
#define INSTPLAN_X86_64_GNU_NASM_

#include <lir/lir.h>
#include <lir/lir_types.h>

lir_block_t* x86_64_gnu_nasm_planner_get_next_func_abi(lir_block_t* entry, lir_block_t* exit, int offset);
lir_block_t* x86_64_gnu_nasm_planner_get_next_sysc_abi(lir_block_t* entry, lir_block_t* exit, int offset);
lir_block_t* x86_64_gnu_nasm_planner_get_func_res(lir_block_t* fn, lir_block_t* exit);

#endif