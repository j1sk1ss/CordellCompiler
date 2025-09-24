#include <hir/opt/ssa.h>

static varver_t* _add_varver(ssa_ctx_t* ctx) {
    varver_t* vv = (varver_t*)mm_malloc(sizeof(varver_t));
    if (!vv) return NULL;
    str_memset(vv, 0, sizeof(varver_t));

    if (!ctx->h) {
        ctx->h = vv;
        return vv;
    }

    varver_t* h = ctx->h;
    while (h->next) h = h->next;
    h->next = vv;
    return vv;
}

static varver_t* _get_varver(long v_id, ssa_ctx_t* ctx) {
    varver_t* h = ctx->h;
    while (h) {
        if (h->v_id == v_id) return h;
        h = h->next;
    }

    return NULL;
}

static int _build_versions(ssa_ctx_t* ctx, sym_table_t* smt) {
    variable_info_t* vh = smt->v.h;
    while (vh) {
        if (!vh->glob) {
            varver_t* vv = _add_varver(ctx);
            stack_push_int(&vv->v, vh->v_id);
            vv->v_id = vh->v_id;
        }
        
        vh = vh->next;
    }

    return 1;
}

static int _iterate_block(cfg_block_t* b, ssa_ctx_t* ctx, sym_table_t* smt) {
    if (!b || b->visited) return 0;
    hir_block_t* hh = b->entry;
    while (hh) {
        switch (hh->op) {
            case HIR_iADD:
            case HIR_iSUB:
            case HIR_iDIV:
            case HIR_iMUL: 
            case HIR_STORE: {
                hir_subject_t* def = hh->farg;
                variable_info_t vi;
                if (VRTB_get_info_id(def->storage.var.v_id, &vi, &smt->v)) {
                    varver_t* vv = _get_varver(vi.v_id, ctx);
                    if (vv) {
                        def->storage.var.v_id = VRTB_add_copy(&vi, &smt->v);
                        stack_push_int(&vv->v, def->storage.var.v_id);
                    }
                }

                break;
            }

            default: {
                hir_subject_t* args[3] = { hh->farg, hh->sarg, hh->targ };
                for (int i = 0; i < 3; i++) {
                    if (args[i] && HIR_is_vartype(args[i]->t)) {
                        varver_t* vv = _get_varver(args[i]->storage.var.v_id, ctx);
                        if (vv) {
                            stack_elem_t se;
                            stack_top_int(&vv->v, &se);
                            args[i]->storage.var.v_id = se.data.intdata;
                        }
                    }
                }

                break;
            }
        }

        if (hh == b->exit) break;
        hh = hh->next;
    }

    _iterate_block(b->jmp, ctx, smt);
    _iterate_block(b->l, ctx, smt);
    return 1;
}

int HIR_SSA_rename(cfg_ctx_t* cctx, ssa_ctx_t* ctx, sym_table_t* smt) {
    _build_versions(ctx, smt);
    cfg_func_t* fh = cctx->h;
    while (fh) {
        cfg_block_t* bh = fh->cfg_head;
        _iterate_block(bh, ctx, smt);
        fh = fh->next;
    }

    return 1;
}
