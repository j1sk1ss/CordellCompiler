#include <hir/opt/ssa.h>

static int _has_phi(cfg_block_t* b, long v_id) {
    hir_block_t* h = b->entry;
    while (h) {
        if (h->op == HIR_PHI && h->farg 
            && h->farg->storage.var.v_id == v_id) {
            return 1;
        }
        if (h == b->exit) break;
        h = h->next;
    }

    return 0;
}

static int _insert_phi_instr(cfg_block_t* b, variable_info_t* vi) {
    token_t tmptkn = { .t_type = vi->type };
    hir_block_t* phi = HIR_create_block(
        HIR_PHI, HIR_SUBJ_STKVAR(vi->v_id, HIR_get_tmptype_tkn(&tmptkn), vi->s_id), NULL, NULL
    );

    HIR_insert_block(phi, b->entry);
    b->entry = phi;
    if (!b->exit) b->exit = phi;
    return 1;
}

int HIR_SSA_insert_phi(cfg_ctx_t* cctx, sym_table_t* smt) {
    for (cfg_func_t* f = cctx->h; f; f = f->next) {
        HIR_CFG_compute_dom(f);
        HIR_CFG_compute_idom(f);
    }

    for (cfg_func_t* f = cctx->h; f; f = f->next) {
        HIR_CFG_compute_domf(f);
    }

    variable_info_t* vh = smt->v.h;
    while (vh) {
        if (!vh->glob) {
            set_t defs;
            set_init(&defs);
            HIR_CFG_collect_defs(vh->v_id, cctx, &defs);

            set_iter_t it;
            set_iter_init(&defs, &it);

            cfg_block_t* b;
            while ((b = (cfg_block_t*)set_iter_next_addr(&it))) {
                set_iter_t fit;
                set_iter_init(&b->df, &fit);

                cfg_block_t* f;
                while ((f = (cfg_block_t*)set_iter_next_addr(&fit))) {
                    if (!_has_phi(f, vh->v_id)) {
                        _insert_phi_instr(f, vh);
                        if (!set_has_addr(&defs, f)) set_add_addr(&defs, f);
                    }
                }
            }

            set_free(&defs);
        }

        vh = vh->next;
    }

    return 1;
}
