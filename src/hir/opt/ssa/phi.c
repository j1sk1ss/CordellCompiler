#include <hir/opt/ssa.h>

static int _has_phi(cfg_block_t* b, long v_id) {
    hir_block_t* h = b->entry;
    while (h) {
        if (h->op == HIR_PHI && h->farg && h->farg->storage.var.v_id == v_id) {
            return 1;
        }
        if (h == b->exit) break;
        h = h->next;
    }
    return 0;
}

static int _insert_phi_instr(cfg_block_t* b, variable_info_t* vi) {
    if (_has_phi(b, vi->v_id)) return 0;

    token_t tmptkn = { .t_type = vi->type };
    hir_block_t* phi = HIR_create_block(
        HIR_PHI, HIR_SUBJ_STKVAR(vi->v_id, HIR_get_stktype(&tmptkn), vi->s_id), NULL, HIR_SUBJ_SET()
    );

    if (b->entry) HIR_insert_block(phi, b->entry->next);
    return 1;
}

int HIR_SSA_insert_phi(cfg_ctx_t* cctx, sym_table_t* smt) {
    list_iter_t it;
    list_iter_hinit(&cctx->funcs, &it);
    cfg_func_t* fb;
    while ((fb = (cfg_func_t*)list_iter_next(&it))) {
        HIR_CFG_compute_dom(fb);
        HIR_CFG_compute_sdom(fb);
        HIR_CFG_compute_domf(fb);
    }

    list_iter_hinit(&smt->v.lst, &it);
    variable_info_t* vh;
    while ((vh = (variable_info_t*)list_iter_next(&it))) {
        // if (vh->glob) continue;

        set_t defs;
        set_init(&defs);
        HIR_CFG_collect_defs_by_id(vh->v_id, cctx, &defs);

        int changed = 0;
        do {
            changed = 0;
            set_iter_t it;
            set_iter_init(&defs, &it);

            cfg_block_t* defb = NULL;
            while ((defb = (cfg_block_t*)set_iter_next_addr(&it))) {
                set_iter_t fit;
                set_iter_init(&defb->domf, &fit);

                cfg_block_t* front = NULL;
                while ((front = (cfg_block_t*)set_iter_next_addr(&fit))) {
                    if (!_has_phi(front, vh->v_id)) {
                        _insert_phi_instr(front, vh);
                        if (!set_has_addr(&defs, front)) {
                            set_add_addr(&defs, front);
                            changed = 1;
                        }
                    }
                }
            }
        } while (changed);

        set_free(&defs);
    }

    return 1;
}
