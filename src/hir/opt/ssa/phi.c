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
        HIR_PHI, HIR_SUBJ_STKVAR(vi->v_id, HIR_get_stktype(&tmptkn), vi->s_id), HIR_SUBJ_SET(), NULL
    );

    if (b->entry) HIR_insert_block(phi, b->entry->next);
    return 1;
}

int HIR_SSA_insert_phi(cfg_ctx_t* cctx, sym_table_t* smt) {
    for (cfg_func_t* f = cctx->h; f; f = f->next) {
        HIR_CFG_compute_dom(f);
        HIR_CFG_compute_idom(f);
        HIR_CFG_compute_domf(f);
    }

    for (variable_info_t* vh = smt->v.h; vh; vh = vh->next) {
        if (vh->glob) continue;

        set_t defs;
        set_init(&defs);
        HIR_CFG_collect_defs(vh->v_id, cctx, &defs);

        int changed;
        do {
            changed = 0;
            set_iter_t it;
            set_iter_init(&defs, &it);

            cfg_block_t* b;
            while ((b = (cfg_block_t*)set_iter_next_addr(&it))) {
                set_iter_t fit;
                set_iter_init(&b->domf, &fit);

                cfg_block_t* f;
                while ((f = (cfg_block_t*)set_iter_next_addr(&fit))) {
                    if (!_has_phi(f, vh->v_id)) {
                        _insert_phi_instr(f, vh);
                        if (!set_has_addr(&defs, f)) {
                            set_add_addr(&defs, f);
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
