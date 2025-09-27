#include <hir/opt/dfg.h>

int HIR_DFG_collect_defs(cfg_ctx_t* cctx) {
    cfg_func_t* fh = cctx->h;
    while (fh) {
        cfg_block_t* bh = fh->cfg_head;
        while (bh) {
            set_init(&bh->def);
            hir_block_t* hh = bh->entry;
            while (hh) {
                if (hh->op == HIR_PHI && HIR_is_vartype(hh->sarg->t) && !HIR_is_tmptype(hh->sarg->t)) {
                    set_add_int(&bh->def, hh->sarg->storage.var.v_id);
                }
                else if (HIR_writeop(hh->op) && HIR_is_vartype(hh->farg->t) && !HIR_is_tmptype(hh->farg->t)) {
                    set_add_int(&bh->def, hh->farg->storage.var.v_id);
                }

                if (hh == bh->exit) break;
                hh = hh->next;
            }

            bh = bh->next;
        }

        fh = fh->next;
    }

    return 1;
}

int HIR_DFG_collect_uses(cfg_ctx_t* cctx) {
    cfg_func_t* fh = cctx->h;
    while (fh) {
        cfg_block_t* bh = fh->cfg_head;
        while (bh) {
            set_init(&bh->use);
            hir_block_t* hh = bh->entry;
            while (hh) {
                if (hh->op != HIR_PHI) {
                    hir_subject_t* args[3] = { hh->farg, hh->sarg, hh->targ };
                    for (int i = HIR_writeop(hh->op); i < 3; i++) {
                        if (args[i] && HIR_is_vartype(args[i]->t) && !HIR_is_tmptype(args[i]->t)) {
                            set_add_int(&bh->use, args[i]->storage.var.v_id);
                        }
                    }
                }
                else {
                    set_iter_t it;
                    set_iter_init(&hh->targ->storage.set.h, &it);
                    int_tuple_t* tpl;
                    while ((tpl = set_iter_next_addr(&it))) {
                        set_add_int(&bh->use, tpl->y);
                    }
                }

                if (hh == bh->exit) break;
                hh = hh->next;
            }

            bh = bh->next;
        }

        fh = fh->next;
    }

    return 1;
}

static int _compute_out(cfg_block_t* cfg) {
    set_t out;
    set_init(&out);

    cfg_block_t* succ[2] = { cfg->l, cfg->jmp };
    for (int i = 0; i < 2; i++) {
        if (!succ[i]) continue;
        set_union_int(&out, &out, &succ[i]->curr_in);
    }

    set_copy_int(&cfg->curr_out, &out);
    set_free(&out);
    return 1;
}

static int _compute_in(cfg_block_t* cfg) {
    set_t tmp;
    set_init(&tmp);

    set_copy_int(&tmp, &cfg->curr_out);
    set_minus_int_set(&tmp, &cfg->def);

    set_free(&cfg->curr_in);
    set_union_int(&cfg->curr_in, &cfg->use, &tmp);

    set_free(&tmp);
    return 1;
}

int HIR_DFG_compute_inout(cfg_ctx_t* cctx) {
    cfg_func_t* fh = cctx->h;
    while (fh) {
        while (1) {
            cfg_block_t* bh = fh->cfg_tail;
            while (bh) {
                _compute_in(bh);
                _compute_out(bh);
                bh = bh->prev;
            }

            int same = 1;
            bh = fh->cfg_tail;
            while (bh) {
                if (!set_equal_int(&bh->curr_in, &bh->prev_in) || !set_equal_int(&bh->curr_out, &bh->prev_out)) {
                    same = 0;
                    break;
                }

                bh = bh->prev;
            }

            if (same) break;
            bh = fh->cfg_tail;
            while (bh) {
                set_copy_int(&bh->prev_in, &bh->curr_in);
                set_copy_int(&bh->prev_out, &bh->curr_out);
                bh = bh->prev;
            }
        }

        fh = fh->next;
    }

    return 1;
}

static int _get_var_parent(long v_id, sym_table_t* smt) {
    variable_info_t vi;
    if (VRTB_get_info_id(v_id, &vi, &smt->v)) {
        return vi.p_id >= 0 ? vi.p_id : v_id;
    }

    return -1;
}

int HIR_DFG_deallocate_vars(cfg_ctx_t* cctx) {
    cfg_func_t* fh = cctx->h;
    while (fh) {
        cfg_block_t* bh = fh->cfg_tail;
        while (bh) {
            bh = bh->prev;
        }

        fh = fh->next;
    }

    return 1;
}
