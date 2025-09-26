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

static int _unload_versions(ssa_ctx_t* ctx) {
    varver_t* h = ctx->h;
    while (h) {
        varver_t* n = h->next;
        mm_free(h);
        h = n;
    }

    return 1;
}

static int _rename_block(hir_block_t* h, ssa_ctx_t* ctx) {
    hir_subject_t* args[3] = { h->farg, h->sarg, h->targ };
    for (int i = HIR_writeop(h->op); i < 3; i++) {
        if (args[i] && HIR_is_vartype(args[i]->t)) {
            varver_t* vv = _get_varver(args[i]->storage.var.v_id, ctx);
            if (vv) {
                stack_elem_t se;
                stack_top_int(&vv->v, &se);
                args[i]->storage.var.v_id = se.data.intdata;
            }
        }
    }

    return 1;
}

static int _iterate_block(cfg_block_t* b, ssa_ctx_t* ctx, long prev_bid, sym_table_t* smt) {
    if (!b || b->visited) return 0;
    b->visited = 1;

    hir_block_t* hh = b->entry;
    while (hh) {
        switch (hh->op) {
            case HIR_PHI: {
                b->visited = 0;
                variable_info_t vi;
                if (VRTB_get_info_id(hh->farg->storage.var.v_id, &vi, &smt->v)) {
                    varver_t* vv = _get_varver(vi.v_id, ctx);
                    if (vv) {
                        stack_elem_t se;
                        stack_top_int(&vv->v, &se);
                        if (hh->targ && se.data.intdata == hh->targ->storage.var.v_id) {
                            break;
                        }
                        
                        int_tuple_t* inf = inttuple_create(prev_bid, se.data.intdata);
                        if (!set_has_inttuple(&hh->sarg->storage.set.h, inf)) {
                            set_add_addr(&hh->sarg->storage.set.h, inf);
                            b->jmp->visited = 0;
                            b->l->visited   = 0;
                        }

                        if (!hh->targ) {
                            hh->targ = HIR_SUBJ_STKVAR(VRTB_add_copy(&vi, &smt->v), hh->farg->t, vi.s_id);
                            stack_push_int(&vv->v, hh->targ->storage.var.v_id);
                        }
                    }
                }

                break;
            }

            case HIR_iADD:
            case HIR_iSUB:
            case HIR_iDIV:
            case HIR_iMUL: 
            case HIR_STORE: {
                variable_info_t vi;
                if (VRTB_get_info_id(hh->farg->storage.var.v_id, &vi, &smt->v)) {
                    varver_t* vv = _get_varver(vi.v_id, ctx);
                    if (vv) {
                        hh->farg = HIR_SUBJ_STKVAR(VRTB_add_copy(&vi, &smt->v), hh->farg->t, vi.s_id);
                        _rename_block(hh, ctx);
                        stack_push_int(&vv->v, hh->farg->storage.var.v_id);
                    }
                }

                break;
            }

            default: _rename_block(hh, ctx); break;
        }

        if (hh == b->exit) break;
        hh = hh->next;
    }

    _iterate_block(b->jmp, ctx, b->id, smt);
    _iterate_block(b->l, ctx, b->id, smt);
    return 1;
}

int HIR_SSA_rename(cfg_ctx_t* cctx, ssa_ctx_t* ctx, sym_table_t* smt) {
    _build_versions(ctx, smt);
    cfg_func_t* fh = cctx->h;
    while (fh) {
        cfg_block_t* bh = fh->cfg_head;
        _iterate_block(bh, ctx, 0, smt);
        fh = fh->next;
    }

    _unload_versions(ctx);
    return 1;
}
