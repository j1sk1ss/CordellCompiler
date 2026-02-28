/* allias.c - Track ownership of address to cell in memory */
#include <hir/cfg.h>

/*
Mark allias (owners to slaves) link for the provided slave & master pair.
Note: Will also take care about master's masters.

```cpl
    function foo() {
        i32 a = 5;   // Slave
        ptr i32 b = ref a; // Master
    }
```

Params:
    - `slave` - Referenced variable.
    - `master` - Reference owner.
    - `ctx` - Allias context.

Return 1 if operation succeed.
*/
static inline void _mark_allias(symbol_id_t slave, symbol_id_t master, sym_table_t* smt) {
    ALLIAS_add_owner(slave, master, &smt->m);
}

/*
Complete owner generation. The last task here is to mark all recursive owners of variables.
For instance:
```cpl
i32 a;
ptr i32 b = ref a; : owner of 'a' :
ptr ptr i32 c = ref b; : owner of 'b' and 'a' :
```

Params:
    - `smt` - Symtable.

Returns 1 if succeeds.
*/
static int _fill_masters(sym_table_t* smt) {
    map_foreach (allias_t* allias, &smt->m.allias) {
        int has_changed = 0;
        do {
            has_changed = 0;
            set_foreach (symbol_id_t owner, &allias->owners) {
                set_t o_owners;
                ALLIAS_get_owners(owner, &o_owners, &smt->m);
                set_foreach (symbol_id_t o_owner, &o_owners) {
                    has_changed = ALLIAS_add_owner(allias->v_id, o_owner, &smt->m) || has_changed;
                }

                set_free(&o_owners);
            }
        } while (has_changed);
    }

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
int _mark_copies(symbol_id_t master, symbol_id_t slave, cfg_ctx_t* cctx, sym_table_t* smt) {
    foreach (cfg_func_t* fb, &cctx->funcs) {
        foreach (cfg_block_t* cb, &fb->blocks) {
            hir_block_t* hh = HIR_get_next(cb->hmap.entry, cb->hmap.exit, 0);
            while (hh) {
                if (
                    (hh->op == HIR_STORE || HIR_is_conv(hh->op)) &&
                    hh->sarg->storage.var.v_id == master) { /* If this is a ST/CNV operation, and we write value */
                                                            /* from a master ID variable, we mark it as a master */
                                                            /* for the provided slave.                           */
                    _mark_allias(slave, hh->farg->storage.var.v_id, smt);
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
            hir_block_t* hh = HIR_get_next(cb->hmap.entry, cb->hmap.exit, 0);
            while (hh) {
                if (hh->op == HIR_REF) {
                    symbol_id_t slave  = hh->sarg->storage.var.v_id;
                    symbol_id_t master = hh->farg->storage.var.v_id;
                    _mark_allias(slave, master, smt);
                    _mark_copies(master, slave, cctx, smt);
                }

                hh = HIR_get_next(hh, cb->hmap.exit, 1);
            }
        }
    }

    _fill_masters(smt);
    return 1;
}
