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

static inline int _get_variable_size(long vid, sym_table_t* smt) {
    variable_info_t vi;
    if (VRTB_get_info_id(vid, &vi, &smt->v)) {
        if (vi.vfs.ptr) return DEFAULT_TYPE_SIZE;
        return _get_ast_type_size(vi.type);
    }

    return DEFAULT_TYPE_SIZE;
}

int x86_64_gnu_nasm_instruction_selection(cfg_ctx_t* cctx, sym_table_t* smt);
int x86_64_gnu_nasm_memory_selection(cfg_ctx_t* cctx, map_t* colors, sym_table_t* smt);
int x86_64_gnu_nasm_caller_saving(cfg_ctx_t* cctx);

#endif