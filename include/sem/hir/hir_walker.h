#ifndef HIR_WALKER_H_
#define HIR_WALKER_H_

#include <sem/misc/warns.h>
#include <sem/hir/hir_data.h>
#include <sem/hir/hir_visitor.h>

typedef struct {
    attention_level_t l;
    hir_visitor_t*    w;
} hir_sem_handler_t;

typedef struct {
    list_t             visitors; /* The walkers list */
    sym_table_t*       smt;      /* Symbolic table   */
    hir_visitors_ctx_t vctx;
} hir_walker_t;

int HIRWLK_init_ctx(hir_walker_t* ctx, sym_table_t* smt);
int HIRWLK_walk(cfg_ctx_t* cctx, hir_walker_t* ctx);
int HIRWLK_register_visitor(unsigned int trg, int (*perform)(HIR_VISITOR_ARGS), hir_walker_t* ctx, attention_level_t l);
int HIRWLK_unload_ctx(hir_walker_t* ctx);

#endif
