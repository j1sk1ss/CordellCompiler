/*
phi.c - Calculate positions of PHI functions for variables and create placeholders for SSA versions
*/

#include <hir/ssa.h>

static int _has_phi(cfg_block_t* b, long v_id) {
    hir_block_t* h = b->hmap.entry;
    while (h) {
        if (h->op == HIR_PHI && h->farg && h->farg->storage.var.v_id == v_id) return 1;
        if (h == b->hmap.exit) break;
        h = h->next;
    }

    return 0;
}

static int _insert_phi_instr(cfg_ctx_t* cctx, cfg_block_t* b, variable_info_t* vi) {
    if (!b->hmap.entry) return 0;

    hir_block_t* hh = b->hmap.entry;
    while (hh) {
        if (hh->op == HIR_PHI && hh->farg->storage.var.v_id == vi->v_id) return 0;
        if (hh == b->hmap.exit) break;
        hh = hh->next;
    }

    hir_block_t* phi = HIR_create_block(HIR_PHI, HIR_SUBJ_STKVAR(vi->v_id, HIR_get_stktype(vi), vi->s_id), NULL, HIR_SUBJ_SET());
    HIR_insert_block_after(phi, b->hmap.entry);
    return 1;
}

int HIR_SSA_insert_phi(cfg_ctx_t* cctx, sym_table_t* smt) {
    map_iter_t mit;
    map_iter_init(&smt->v.vartb, &mit);
    variable_info_t* vh;
    while (map_iter_next(&mit, (void**)&vh)) {
        set_t defs;
        set_init(&defs);
        HIR_CFG_collect_defs_by_id(vh->v_id, cctx, &defs);

        int changed = 0;
        do {
            changed = 0;
            set_iter_t it;
            set_iter_init(&defs, &it);
            cfg_block_t* defb;
            while (set_iter_next(&it, (void**)&defb)) {
                set_iter_t fit;
                set_iter_init(&defb->domf, &fit);
                cfg_block_t* front;
                while (set_iter_next(&fit, (void**)&front)) {
                    if (!_has_phi(front, vh->v_id)) {
                        _insert_phi_instr(cctx, front, vh);
                        if (set_add(&defs, front)) changed = 1;
                    }
                }
            }
        } while (changed);

        set_free(&defs);
    }

    return 1;
}
