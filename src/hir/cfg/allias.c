/* allias.c - Track ownership of address to cell in memory */
#include <hir/cfg.h>

/*
Mark allias (owners to slaves) link for the provided slave & master pair.
Note: Will also take care about master's masters.

```c
    void foo() {
        int a = 5;   // Slave
        int* b = &a; // Master
    }
```

Params:
    - `slave` - Referenced variable.
    - `master` - Reference owner.
    - `ctx` - Allias context.

Return 1 if operation succeed.
*/
static int _mark_allias(long slave, long master, allias_map_t* ctx) {
    set_t owners;
    ALLIAS_get_owners(master, &owners, ctx); /* We should inquier all masters for the provided master   */
    set_add(&owners, (void*)master);         /* Note: We add the current master to the masters set      */
                                             /*       to complete the final masters set of the provided */
                                             /*       slave.                                            */

    set_foreach (long master_owner, &owners) {
        ALLIAS_add_owner(slave, master_owner, ctx);
    }

    set_free(&owners);
    return 1;
}

/*
Recursivly mark all masters for the provided slave.
Params:
    - `master` - Master variable.
    - `slave` - Slave variable.
    - `cctx` - CFG context.
    - `smt` - Symtable.

Return 1 if operation succeed.
*/
int _mark_copies(long master, long slave, cfg_ctx_t* cctx, sym_table_t* smt) {
    foreach (cfg_func_t* fb, &cctx->funcs) {
        foreach (cfg_block_t* cb, &fb->blocks) {
            hir_block_t* hh = HIR_get_next(cb->hmap.entry, cb->hmap.exit, 0);
            while (hh) {
                if (hh->op == HIR_STORE && hh->sarg->storage.var.v_id == master) { /* If this is a STORE operation, and we write value  */
                                                                                   /* from a master ID variable, we mark it as a master */
                                                                                   /* for the provided slave.                           */
                    ALLIAS_add_owner(slave, hh->farg->storage.var.v_id, &smt->m);
                    _mark_copies(hh->farg->storage.var.v_id, slave, cctx, smt);
                }

                hh = HIR_get_next(hh, cb->hmap.exit, 1);
            }
        }
    }

    return 1;
}

int HIR_CFG_make_allias(cfg_ctx_t* cctx, sym_table_t* smt) {
    foreach (cfg_func_t* fb, &cctx->funcs) {
        foreach (cfg_block_t* cb, &fb->blocks) {
            hir_block_t* hh = cb->hmap.entry;
            while (hh) {
                if (hh->op == HIR_REF) {
                    long slave  = hh->sarg->storage.var.v_id;
                    long master = hh->farg->storage.var.v_id;
                    _mark_allias(slave, master, &smt->m);
                    _mark_copies(master, slave, cctx, smt);
                }

                if (hh == cb->hmap.exit) break;
                hh = hh->next;
            }
        }
    }

    return 1;
}
