#ifndef HIR_DATA_H_
#define HIR_DATA_H_

#include <std/str.h>
#include <hir/hir.h>
#include <hir/cfg.h>

typedef enum {
    SETPOS_INST  = 1 << 0,
    IF_INST      = 1 << 1,
    LDREF_INST   = 1 << 2,
    GDREF_INST   = 1 << 3,
    UNKNOWN_INST = 1 << 4,
} hir_instruction_type_t;

typedef struct {
    struct {
        string_t* file;
        long      column;
        long      line;
    } prev_location;
    struct {
        string_t* file;
        long      column;
        long      line;
    } curr_location;
} hir_visitors_ctx_t;

#define HIR_VISITOR_ARGS     hir_block_t* b, cfg_block_t* bb, sym_table_t* smt, hir_visitors_ctx_t* ctx
#define HIR_VISITOR_ARGS_USE (void)b; (void)bb; (void)smt; (void)ctx;

#endif
