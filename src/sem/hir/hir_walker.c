#include <sem/hir/hir_walker.h>

/*
Create a semantic handler for a walker.
Params:
    - `v` - HIR visitor.
    - `l` - Handler level.

Returns the semantic handler.
*/
static hir_sem_handler_t* _create_sem_handler(hir_visitor_t* v, attention_level_t l) {
    hir_sem_handler_t* h = (hir_sem_handler_t*)mm_malloc(sizeof(hir_sem_handler_t));
    if (!h) return NULL;
    h->w = v;
    h->l = l;
    return h;
}

/*
Unload the semantic handler.
Params:
    - `h` - Semantic handler.

Returns 1 if succeeds.
*/
static int _unload_sem_handler(hir_sem_handler_t* h) {
    HIRVIS_unload_visitor(h->w);
    return mm_free(h);
}

int HIRWLK_register_visitor(unsigned int trg, int (*perform)(HIR_VISITOR_ARGS), hir_walker_t* ctx, attention_level_t l) {
    hir_visitor_t* v = HIRVIS_create_visitor(trg, perform);
    if (!v) return 0;
    hir_sem_handler_t* w = _create_sem_handler(v, l);
    if (!w) {
        _unload_sem_handler(w);
        return 0;
    }

    return list_add(&ctx->visitors, w);
}

int HIRWLK_init_ctx(hir_walker_t* ctx, dag_ctx_t* dctx, sym_table_t* smt) {
    str_memset(ctx, 0, sizeof(hir_walker_t));
    map_init(&ctx->vctx.definitions, MAP_NO_CMP);
    ctx->smt = smt;
    ctx->vctx.dctx = dctx;
    return list_init(&ctx->visitors);
}

/*
Get an instruction type based on the provided operation type.
Params:
    - `t` - Operation type.

Returns an instruction type.
*/
static hir_instruction_type_t _get_instruction_type(hir_operation_t t) {
    switch (t) {
        case HIR_FCLL:
        case HIR_ECLL:
        case HIR_UFCLL:       return CALL_INST;
        case HIR_STORE_FCLL:
        case HIR_STORE_ECLL:
        case HIR_STORE_UFCLL: return RET_CALL_INST;
        case HIR_PHI:         return PHI_INST;
        case HIR_IFOP2:       return IF_INST;
        case HIR_LDREF:       return LDREF_INST;
        case HIR_GDREF:       return GDREF_INST;
        case HIR_SETPOS:      return SETPOS_INST;
        default: break;
    }

    return UNKNOWN_INST;
}

/*
Perform a walk thru the HIR. This is a linear approach, that allows us
to use a analytic symtables for the complex static analysis.
Params:
    - `cctx` - CFG context.
    - `ctx` - Walker context.

Returns 1 if succeeds. Otherwise returns 0 - Semantic block of a compilation.
*/
static int _cfg_walk(cfg_ctx_t* cctx, hir_walker_t* ctx) {
    if (!cctx || !ctx) return 0;
    
    foreach (cfg_func_t* fb, &cctx->funcs) {
        foreach (cfg_block_t* bb, &fb->blocks) {
            hir_block_t* hb = HIR_get_next(bb->hmap.entry, bb->hmap.exit, 0);
            while (hb) {
                foreach (hir_sem_handler_t* v, &ctx->visitors) {
                    if (_get_instruction_type(hb->op) & v->w->trg) {
                        int res = v->w->perform(hb, bb, ctx->smt, &ctx->vctx);
                        if (
                            !res && 
                            v->l == ATTENTION_BLOCK_LEVEL
                        ) return -1;
                    }
                }
                
                hb = HIR_get_next(hb, bb->hmap.exit, 1);
            }
        }
    }

    return 1;
}

int HIRWLK_walk(cfg_ctx_t* cctx, hir_walker_t* ctx) {
    return _cfg_walk(cctx, ctx);
}

static int _free_definitions_entry(list_t* l) {
    list_free(l);
    return mm_free(l);
}

int HIRWLK_unload_ctx(hir_walker_t* ctx) {
    map_free_force_op(&ctx->vctx.definitions, (int (*)(void*))_free_definitions_entry);
    list_free_force_op(&ctx->visitors, (int (*)(void *))_unload_sem_handler);
    return mm_free(ctx);
}
