/*
phi.c - Calculate positions of PHI functions for variables and create placeholders for SSA versions
*/

#include <hir/opt/ssa.h>

static int _has_phi(cfg_block_t* b, long v_id) {
    hir_block_t* h = b->entry;
    while (h) {
        if (h->op == HIR_PHI && h->farg && h->farg->storage.var.v_id == v_id) return 1;
        if (h == b->exit) break;
        h = h->next;
    }

    return 0;
}

static int _insert_phi_instr(cfg_ctx_t* cctx, cfg_block_t* b, variable_info_t* vi) {
    if (!b->entry) return 0;

    set_iter_t pit;
    set_iter_init(&b->pred, &pit);
    cfg_block_t* p;
    while ((p = set_iter_next_addr(&pit))) {
        if (p->entry->op == HIR_PHI) return 0;
    }

    token_t tmptkn = { .t_type = vi->type };
    hir_block_t* phi = HIR_create_block(
        HIR_PHI, HIR_SUBJ_STKVAR(vi->v_id, HIR_get_stktype(&tmptkn), vi->s_id), NULL, HIR_SUBJ_SET()
    );

    HIR_insert_block_before(phi, b->entry);
    cfg_block_t* phiblock = CFG_create_cfg_block(phi);
    phiblock->id = cctx->cid++;
    CFG_insert_cfg_block(b->pfunc, phiblock, b);
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
        set_t defs;
        set_init(&defs);
        HIR_CFG_collect_defs_by_id(vh->v_id, cctx, &defs);

        int changed = 0;
        do {
            changed = 0;
            set_iter_t it;
            set_iter_init(&defs, &it);
            cfg_block_t* defb;
            while ((defb = (cfg_block_t*)set_iter_next_addr(&it))) {
                set_iter_t fit;
                set_iter_init(&defb->domf, &fit);
                cfg_block_t* front;
                while ((front = (cfg_block_t*)set_iter_next_addr(&fit))) {
                    if (!_has_phi(front, vh->v_id)) {
                        _insert_phi_instr(cctx, front, vh);
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
