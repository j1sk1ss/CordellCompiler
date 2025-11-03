#ifndef INSTSEL_X86_64_GNU_NASM_
#define INSTSEL_X86_64_GNU_NASM_

#include <std/map.h>
#include <std/list.h>
#include <std/stackmap.h>

#include <symtab/symtab.h>
#include <prep/token_types.h>
#include <hir/cfg.h>

#define DEFAULT_TYPE_SIZE 8

int x86_64_gnu_nasm_instruction_selection(cfg_ctx_t* cctx, sym_table_t* smt);
int x86_64_gnu_nasm_register_selection(cfg_ctx_t* cctx, map_t* colors, sym_table_t* smt);

#endif