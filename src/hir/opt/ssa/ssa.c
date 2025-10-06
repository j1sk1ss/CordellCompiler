/*
ssa.c - Transfer input HIR program (with PHI placeholders) into the SSA form
*/

#include <hir/opt/ssa.h>

static int _add_varver(list_t* vers, long id, long cid) {
    varver_t* vv = (varver_t*)mm_malloc(sizeof(varver_t));
    if (!vv) return 0;
    str_memset(vv, 0, sizeof(varver_t));
    vv->v_id    = id;
    vv->curr_id = cid;
    list_add(vers, vv);
    return 1;
}

static varver_t* _get_varver(long v_id, ssa_ctx_t* ctx) {
    list_iter_t it;
    list_iter_hinit(&ctx->vers, &it);
    varver_t* hi;
    while ((hi = (varver_t*)list_iter_next(&it))) {
        if (hi->v_id == v_id) return hi;
    }

    return NULL;
}

static int _rename_block(hir_block_t* h, ssa_ctx_t* ctx) {
    hir_subject_t* args[3] = { h->farg, h->sarg, h->targ };
    for (int i = HIR_writeop(h->op); i < 3; i++) {
        if (args[i] && HIR_is_vartype(args[i]->t)) {
            varver_t* vv = _get_varver(args[i]->storage.var.v_id, ctx);
            if (vv) args[i]->storage.var.v_id = vv->curr_id;
        }
    }

    return 1;
}

static int _iterate_block(cfg_block_t* b, ssa_ctx_t* ctx, long prev_bid, sym_table_t* smt) {
    if (!b || set_has(&b->visitors, (void*)prev_bid)) return 0;

    hir_block_t* hh = b->entry;
    while (hh) {
        switch (hh->op) {
            case HIR_PHI: {
                variable_info_t vi;
                if (VRTB_get_info_id(hh->farg->storage.var.v_id, &vi, &smt->v)) {
                    varver_t* vv = _get_varver(vi.v_id, ctx);
                    if (vv) {
                        if (hh->sarg && vv->curr_id == hh->sarg->storage.var.v_id) break;
                        int_tuple_t* inf = inttuple_create(prev_bid, vv->curr_id);
                        if (!set_has_inttuple(&hh->targ->storage.set.h, inf)) set_add(&hh->targ->storage.set.h, inf);
                        else inttuple_free(inf);

                        if (!hh->sarg) {
                            hh->sarg = HIR_SUBJ_STKVAR(VRTB_add_copy(&vi, &smt->v), hh->farg->t, vi.s_id);
                            vv->curr_id = hh->sarg->storage.var.v_id;
                        }
                    }
                }

                break;
            }

            default: {
                variable_info_t vi;
                if (VRTB_get_info_id(hh->farg->storage.var.v_id, &vi, &smt->v) && HIR_writeop(hh->op)) {
                    varver_t* vv = _get_varver(vi.v_id, ctx);
                    if (vv) {
                        hh->farg = HIR_SUBJ_STKVAR(VRTB_add_copy(&vi, &smt->v), hh->farg->t, vi.s_id);
                        vv->curr_id = hh->farg->storage.var.v_id;
                    }
                }

                _rename_block(hh, ctx);
                break;
            }
        }

        if (hh == b->exit) break;
        hh = hh->next;
    }

    set_add(&b->visitors, (void*)prev_bid);

    if (!b->jmp || !b->l) {
        _iterate_block(b->jmp, ctx, b->id, smt);
        _iterate_block(b->l, ctx, b->id, smt);
    }
    else {
        list_t saved;
        list_init(&saved);
        
        list_iter_t it;
        list_iter_hinit(&ctx->vers, &it);
        varver_t* s;
        while ((s = (varver_t*)list_iter_next(&it))) {
            _add_varver(&saved, s->v_id, s->curr_id);
        }

        _iterate_block(b->jmp, ctx, b->id, smt);
        list_free_force(&ctx->vers);
        
        list_iter_hinit(&saved, &it);
        while ((s = (varver_t*)list_iter_next(&it))) {
            _add_varver(&ctx->vers, s->v_id, s->curr_id);
        }
        
        _iterate_block(b->l, ctx, b->id, smt);
        list_free_force(&saved);
    }

    return 1;
}

int HIR_SSA_rename(cfg_ctx_t* cctx, ssa_ctx_t* ctx, sym_table_t* smt) {
    map_iter_t mit;
    map_iter_init(&smt->v.vartb, &mit);
    variable_info_t* vh;
    while (map_iter_next(&mit, (void**)&vh)) {
        if (vh->glob) continue;
        _add_varver(&ctx->vers, vh->v_id, vh->v_id);
    }

    list_iter_t it;
    list_iter_hinit(&cctx->funcs, &it);
    cfg_func_t* fb;
    while ((fb = (cfg_func_t*)list_iter_next(&it))) {
        _iterate_block(list_get_head(&fb->blocks), ctx, 0, smt);
    }

    list_free_force(&ctx->vers);
    return 1;
}
