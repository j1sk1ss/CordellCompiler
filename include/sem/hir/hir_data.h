#ifndef HIR_DATA_H_
#define HIR_DATA_H_

#include <std/map.h>
#include <std/str.h>
#include <hir/hir.h>
#include <hir/cfg.h>
#include <hir/dag.h>

typedef enum {
    SETPOS_INST  = 1 << 0,
    IF_INST      = 1 << 1,
    LDREF_INST   = 1 << 2,
    GDREF_INST   = 1 << 3,
    PHI_INST     = 1 << 4,
    CALL         = 1 << 5,
    RET_CALL     = 1 << 6,
    UNKNOWN_INST = 1 << 7,
} hir_instruction_type_t;

typedef struct {
    struct {
        string_t* file;
        long      column;
        long      line;
    } prev_location;
    struct {
        string_t* file;        /* Current file name                                  */
        long      column;      /* Current column in the file                         */
        long      line;        /* Current line in the file                           */
    } curr_location;
    map_t         definitions; /* map of id:list, possible definitions of a variable */
    dag_ctx_t*    dctx;
} hir_visitors_ctx_t;

#define HIR_VISITOR_ARGS     hir_block_t* b, cfg_block_t* bb, sym_table_t* smt, hir_visitors_ctx_t* ctx
#define HIR_VISITOR_ARGS_USE (void)b; (void)bb; (void)smt; (void)ctx;

#endif
