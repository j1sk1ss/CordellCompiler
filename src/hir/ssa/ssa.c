/* ssa.c - Transfer input HIR program (with PHI placeholders) into the SSA form
*/

#include <hir/ssa.h>

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
    foreach (varver_t* hi, &ctx->vers) {
        if (hi->v_id == v_id) return hi;
    }

    return NULL;
}

static int _rename_block(hir_block_t* h, ssa_ctx_t* ctx) {
    hir_subject_t* args[3] = { h->farg, h->sarg, h->targ };
    for (int i = HIR_writeop(h->op); i < 3; i++) {
        if (!args[i]) continue;
        if (HIR_is_vartype(args[i]->t)) {
            varver_t* vv = _get_varver(args[i]->storage.var.v_id, ctx);
            if (vv) args[i]->storage.var.v_id = vv->curr_id;
        }
        else if (args[i]->t == HIR_ARGLIST) {
            foreach (hir_subject_t* s, &args[i]->storage.list.h) {
                if (!HIR_is_vartype(s->t)) continue;
                varver_t* vv = _get_varver(s->storage.var.v_id, ctx);
                if (vv) s->storage.var.v_id = vv->curr_id;
            }
        }
    }

    return 1;
}

static int _insert_phi_preamble(cfg_block_t* block, long bid, int a, int b, sym_table_t* smt) {
    if (a == b) return 1;
    variable_info_t avi, bvi;
    if (!VRTB_get_info_id(a, &avi, &smt->v) || !VRTB_get_info_id(b, &bvi, &smt->v)) {
        return 0;
    }

    set_iter_t it;
    set_iter_init(&block->pred, &it);
    cfg_block_t* trg;
    while (set_iter_next(&it, (void**)&trg)) {
        if (trg->id != bid) continue;
        hir_block_t* union_command = HIR_create_block(
            HIR_PHI_PREAMBLE, 
            HIR_SUBJ_STKVAR(avi.v_id, HIR_get_stktype(&avi)), 
            HIR_SUBJ_STKVAR(bvi.v_id, HIR_get_stktype(&bvi)), 
            NULL
        );
        
        if (trg->hmap.exit) HIR_insert_block_before(union_command, trg->hmap.exit);
        else {
            HIR_CFG_append_hir_block_back(trg, union_command);
            HIR_insert_block_before(union_command, trg->l->hmap.entry);
        }

        if (trg->hmap.entry == trg->hmap.exit) {
            trg->hmap.entry = union_command;
        }
        
        break;
    }

    return 1;
}

static int _iterate_block(cfg_block_t* b, ssa_ctx_t* ctx, long prev_bid, sym_table_t* smt) {
    if (!b || set_has(&b->visitors, (void*)prev_bid)) return 0;

    hir_block_t* hh = b->hmap.entry;
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

                        int prev_id = vv->curr_id;
                        int future_id = 0;
                        if (hh->sarg) future_id = hh->sarg->storage.var.v_id;
                        else {
                            hh->sarg = HIR_SUBJ_STKVAR(VRTB_add_copy(&vi, &smt->v), hh->farg->t);
                            vv->curr_id = hh->sarg->storage.var.v_id;
                            future_id = vv->curr_id;
                        }

                        _insert_phi_preamble(b, prev_bid, future_id, prev_id, smt);
                    }
                }

                break;
            }

            default: {
                _rename_block(hh, ctx);
                
                variable_info_t vi;
                if (
                    hh->farg && HIR_is_vartype(hh->farg->t) && 
                    VRTB_get_info_id(hh->farg->storage.var.v_id, &vi, &smt->v) && 
                    HIR_writeop(hh->op)
                ) {
                    varver_t* vv = _get_varver(vi.v_id, ctx);
                    if (vv) {
                        hir_subject_type_t tmp_t = hh->farg->t;
                        if (hh->farg->home == hh) HIR_unload_subject(hh->farg); 
                        hh->farg = HIR_SUBJ_STKVAR(VRTB_add_copy(&vi, &smt->v), tmp_t);
                        array_info_t ai;
                        if (ARTB_get_info(vi.v_id, &ai, &smt->a)) {
                            ARTB_add_copy(hh->farg->storage.var.v_id, &ai, &smt->a);
                        }

                        vv->curr_id = hh->farg->storage.var.v_id;
                    }
                }

                break;
            }
        }

        if (hh == b->hmap.exit) break;
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
        
        foreach (varver_t* s, &ctx->vers) {
            _add_varver(&saved, s->v_id, s->curr_id);
        }

        _iterate_block(b->jmp, ctx, b->id, smt);
        list_free_force(&ctx->vers);
        
        foreach (varver_t* s, &saved) {
            _add_varver(&ctx->vers, s->v_id, s->curr_id);
        }
        
        _iterate_block(b->l, ctx, b->id, smt);
        list_free_force(&saved);
    }

    return 1;
}

int HIR_SSA_rename(cfg_ctx_t* cctx, ssa_ctx_t* ctx, sym_table_t* smt) {
    map_foreach (variable_info_t* vh, &smt->v.vartb) {
        if (vh->ro || TKN_istmp_type(vh->type)) continue;
        _add_varver(&ctx->vers, vh->v_id, vh->v_id);
    }

    foreach (cfg_func_t* fb, &cctx->funcs) {
        if (!fb->used) continue;
        _iterate_block(list_get_head(&fb->blocks), ctx, 0, smt);
    }

    list_free_force(&ctx->vers);
    return 1;
}
