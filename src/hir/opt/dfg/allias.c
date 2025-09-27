#include <hir/opt/dfg.h>

static int _mark_allias(long v_id, long owner_id, allias_map_t* ctx) {
    set_t owners;
    set_init(&owners);

    ALLIAS_get_owners(owner_id, &owners, ctx);    
    set_add_int(&owners, owner_id);

    set_iter_t it;
    set_iter_init(&owners, &it);
    long oid;
    while ((oid = set_iter_next_int(&it)) >= 0) {
        ALLIAS_add_owner(v_id, oid, ctx);
    }

    set_free(&owners);
    return 1;
}

int _mark_copies(long owner_id, long owned_id, cfg_ctx_t* cctx, sym_table_t* smt) {
    cfg_func_t* fh = cctx->h;
    while (fh) {
        cfg_block_t* bh = fh->cfg_tail;
        while (bh) {
            hir_block_t* hh = bh->entry;
            while (hh) {
                if (hh->op == HIR_STORE) {
                    long src = hh->sarg->storage.var.v_id;
                    long dst = hh->farg->storage.var.v_id;
                    if (src == owner_id) ALLIAS_add_owner(owned_id, dst, &smt->m);
                }

                if (hh == bh->exit) break;
                hh = hh->next;
            }

            bh = bh->prev;
        }

        fh = fh->next;
    }

    return 1;
}

int HIR_DFG_make_allias(cfg_ctx_t* cctx, sym_table_t* smt) {
    cfg_func_t* fh = cctx->h;

    while (fh) {
        cfg_block_t* bh = fh->cfg_tail;
        while (bh) {
            hir_block_t* hh = bh->entry;
            while (hh) {
                if (hh->op == HIR_REF) {
                    long slave  = hh->sarg->storage.var.v_id;
                    long master = hh->farg->storage.var.v_id;
                    _mark_allias(slave, master, &smt->m);
                    _mark_copies(master, slave, cctx, smt);
                }

                if (hh == bh->exit) break;
                hh = hh->next;
            }

            bh = bh->prev;
        }
        
        fh = fh->next;
    }

    return 1;
}

