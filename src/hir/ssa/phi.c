/* phi.c - Calculate positions of PHI functions for variables and create placeholders for SSA versions */
#include <hir/ssa.h>

/*
Insert a PHI instruction to the BasicBlock. 
Note: This function doesn't check is another phi function 
      with the save variable is presented in the Basic Block.
Params:
    - `b` - Basic Block.
    - `vi` - Variable ID.
             Note: This variable must be a 'base' variable.
                   This means that variable mustn't be a copy.

Returns 1 if PHI function insert succeed. Otherwise will return 0.
*/
static int _insert_phi_instr(cfg_block_t* b, variable_info_t* vi) {
    if (!b || !b->hmap.entry) return 0;
    hir_block_t* hh = HIR_get_next(b->hmap.entry, b->hmap.exit, 0);
    while (hh) {
        if (hh->op == HIR_PHI && hh->farg->storage.var.v_id == vi->v_id) return 0;
        hh = HIR_get_next(hh, b->hmap.exit, 1);
    }

    hir_subject_t* phi_set = HIR_SUBJ_SET();
    hir_block_t* phi = HIR_create_block(HIR_PHI, HIR_SUBJ_STKVAR(vi->v_id, HIR_get_stktype(vi), 0), NULL, phi_set);
    if (!phi || !phi_set) {
        HIR_unload_subject(phi_set);
        HIR_unload_blocks(phi);
        return 0;
    }

    HIR_insert_block_after(phi, b->hmap.entry);
    set_add(&b->phi, (void*)vi->v_id);
    return 1;
}

/*
Collect all base blocks where occurs definition of provided v_id.
Params:
    - `v_id` - Variable ID.
    - `cctx` - CFG func.
    - `out` - Output set where collected all base-blocks.

Return 1 if success, otherwise 0.
*/
static int _collect_defs_by_id(long v_id, cfg_ctx_t* cctx, set_t* out) {
    foreach (cfg_func_t* fb, &cctx->funcs) {
        if (!fb->used) continue;
        foreach (cfg_block_t* cb, &fb->blocks) {
            int has_def = 0;
            hir_block_t* hh = HIR_get_next(cb->hmap.entry, cb->hmap.exit, 0);
            while (hh) {
                if (HIR_writeop(hh->op)) {
                    if (
                        hh->farg &&                     /* - If the first argument is presented */
                        HIR_is_vartype(hh->farg->t) &&  /* - If this is a variable              */
                        !HIR_is_tmptype(hh->farg->t)    /* - and not a tmp type                 */
                    ) {
                        if (hh->farg->storage.var.v_id == v_id) {
                            has_def = 1;
                            break;
                        }
                    }
                }
                
                hh = HIR_get_next(hh, cb->hmap.exit, 1);
            }

            if (has_def) {
                set_add(out, cb);
            }
        }
    }

    return 1;
}

int HIR_SSA_insert_phi(cfg_ctx_t* cctx, sym_table_t* smt) {
    map_foreach (variable_info_t* vh, &smt->v.vartb) {
        set_t defs;
        set_init(&defs, SET_NO_CMP); /* Collect all blocks from the provided CFG context */
                                     /* where current variable is defined / assigned.    */
        if (!_collect_defs_by_id(vh->v_id, cctx, &defs)) {
            set_free(&defs);
            return 0;
        }

        int changed = 0;
        do {
            changed = 0;
            set_foreach (cfg_block_t* defb, &defs) {
                set_foreach (cfg_block_t* front, &defb->domf) {
                    if (!set_has(&front->phi, (void*)vh->v_id)) {
                        if (!_insert_phi_instr(front, vh)) {
                            print_error("PHI function can't be append to the 'front' block!");
                            set_free(&defs);
                            return 0;
                        }

                        if (set_add(&defs, front)) {
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
