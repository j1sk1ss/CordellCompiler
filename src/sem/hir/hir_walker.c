#include <sem/hir/hir_walker.h>

/* Create a semantic handler for a walker.
Params:
    - `v` - HIR visitor.
    - `l` - Handler level.

Returns the semantic handler */
static hir_sem_handler_t* _create_sem_handler(hir_visitor_t* v, attention_level_t l) {
    hir_sem_handler_t* h = (hir_sem_handler_t*)mm_malloc(sizeof(hir_sem_handler_t));
    if (!h) return NULL;
    h->w = v;
    h->l = l;
    return h;
}

/* Unload the semantic handler.
Params:
    - `h` - Semantic handler.

Returns 1 if succeeds */
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

int HIRWLK_init_ctx(hir_walker_t* ctx, dag_ctx_t* dctx, hir_ctx_t* hctx, sym_table_t* smt) {
    str_memset(ctx, 0, sizeof(hir_walker_t));
    map_init(&ctx->vctx.definitions, MAP_NO_CMP);
    ctx->smt       = smt;
    ctx->vctx.dctx = dctx;
    ctx->vctx.z3   = NULL;
    ctx->vctx.dump = tmpfile();
    DUMP_format_hirctx(hctx, smt, 0, 0, ctx->vctx.dump);
    return list_init(&ctx->visitors);
}

/* Get an instruction type based on the provided operation type.
Params:
    - `t` - Operation type.

Returns an instruction type */
static hir_instruction_type_t _get_instruction_type(hir_operation_t t) {
    switch (t) {
        case HIR_REF:         return REF_INST;
        case HIR_SYSC:        case HIR_FCLL:       case HIR_ECLL:
        case HIR_UFCLL:       return CALL_INST;
        case HIR_STORE_SYSC:  case HIR_STORE_FCLL: case HIR_STORE_ECLL:
        case HIR_STORE_UFCLL: return RET_CALL_INST;
        case HIR_PHI:         return PHI_INST;
        case HIR_IFOP2:       return IF_INST;
        case HIR_LDREF:       return LDREF_INST;
        case HIR_GDREF:       return GDREF_INST;
        case HIR_SETPOS:      return SETPOS_INST;
        case HIR_VARDECL:     return DECL_INST;
        default: {
            if (HIR_is_writeop(t)) return STMT_INST;
            break;
        }
    }

    return UNKNOWN_INST;
}

/* Perform a recursive walk from a CFG block thru its successors.
Params:
    - `bb` - CFG block.
    - `ctx` - Walker context.

Returns -1 if a blocking visitor fails, otherwise non-negative. */
static int _cfg_block_walk(cfg_block_t* bb, hir_walker_t* ctx) {
    if (!bb || !ctx) return 0;
    if (bb->visited) return 1;
    bb->visited = 1;

    iterate_hir_instructions (bb) {
        foreach (hir_sem_handler_t* v, &ctx->visitors) {
            if (_get_instruction_type(hh->op) & v->w->trg) {
                int res = v->w->perform(hh, bb, ctx->smt, &ctx->vctx);
                if (
                    !res && 
                    v->l == ATTENTION_BLOCK_LEVEL
                ) return -1;
            }
        }
    }

    if (_cfg_block_walk(bb->l, ctx) < 0)   return -1;
    if (_cfg_block_walk(bb->jmp, ctx) < 0) return -1;
    return 1;
}

/* Perform a recursive walk thru the HIR. This approach follows CFG edges
from each function entry block, so unreachable blocks are skipped and loops
are guarded by the block's visited flag.
Params:
    - `cctx` - CFG context.
    - `ctx` - Walker context.

Returns 1 on success, otherwise 0 */
static int _cfg_walk(cfg_ctx_t* cctx, hir_walker_t* ctx) {
    if (!cctx || !ctx) return 0;

    foreach (cfg_func_t* fb, &cctx->funcs) {
        foreach (cfg_block_t* bb, &fb->blocks) {
            bb->visited = 0;
        }
    }

    foreach (cfg_func_t* fb, &cctx->funcs) {
        if (_cfg_block_walk(list_get_head(&fb->blocks), ctx) < 0) return -1;
    }

    return 1;
}

int HIRWLK_walk(cfg_ctx_t* cctx, hir_walker_t* ctx) {
    if (!ctx) return 0;
    ctx->vctx.z3 = Z3A_create(cctx, ctx->smt);
    int result   = _cfg_walk(cctx, ctx);
    HIR_CFG_cleanup_navigation(cctx);
    Z3A_unload(ctx->vctx.z3);
    ctx->vctx.z3 = NULL;
    return result;
}

static int _free_definitions_entry(list_t* l) {
    list_free(l);
    return mm_free(l);
}

int HIRWLK_unload_ctx(hir_walker_t* ctx) {
    map_free_force_op(&ctx->vctx.definitions, (int (*)(void*))_free_definitions_entry);
    list_free_force_op(&ctx->visitors, (int (*)(void*))_unload_sem_handler);
    fclose(ctx->vctx.dump);
    return mm_free(ctx);
}
