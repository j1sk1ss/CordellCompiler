#include <lir/copyprop.h>

/*
Add variables referenced by a subject to the USE set.
Params:
    - `use` - USE set to update.
    - `arg` - LIR subject to inspect.

Returns 1 if subject was handled, otherwise 0.
*/
static int _mark_used_var(set_t* use, lir_subject_t* arg) {
    if (!arg) return 0;
    if (arg->t == LIR_VARIABLE) {
        set_add(use, arg->storage.var.v_id);
        return 1;
    }

    if (arg->t == LIR_ARGLIST) {
        foreach (lir_subject_t* it, &arg->storage.list.h) {
            if (it->t == LIR_VARIABLE) {
                set_add(use, it->storage.var.v_id);
            }
        }
    }

    return 1;
}

int LIR_drop_unused_variables(cfg_ctx_t* cctx) {
    foreach (cfg_func_t* fb, &cctx->funcs) {
        set_t use, def;
        map_t def_trg;
        map_init(&def_trg, MAP_NO_CMP);
        set_init(&use, SET_NO_CMP);
        set_init(&def, SET_NO_CMP);
        foreach (cfg_block_t* bb, &fb->blocks) {
            lir_block_t* lh = LIR_get_next(bb->lmap.entry, bb->lmap.exit, 0);
            while (lh) {
                if (!lh->unused) {
                    iterate_lir_args(lir_subject_t* arg, lh, LIR_is_writeop(lh->op)) {
                        _mark_used_var(&use, arg);
                    }

                    switch (lh->op) {
                        case LIR_TF64: case LIR_TF32: 
                        case LIR_TI64: case LIR_TI32: case LIR_TI16: case LIR_TI8: 
                        case LIR_TU64: case LIR_TU32: case LIR_TU16: case LIR_TU8:
                        case LIR_iMOV: case LIR_aMOV: {                        
                            if (lh->farg && lh->farg->t == LIR_VARIABLE) {
                                set_add(&def, lh->farg->storage.var.v_id);
                                map_put(&def_trg, lh->farg->storage.var.v_id, lh);
                            }

                            break;
                        }
                        default: break;
                    }
                }

                lh = LIR_get_next(lh, bb->lmap.exit, 1);
            }
        }

        set_minus_set(&def, &use);
        set_foreach (symbol_id_t unused, &def) {
            lir_block_t* dst;
            if (map_get(&def_trg, unused, (void**)&dst)) dst->unused = 1;
        }

        map_free(&def_trg);
        set_free(&use);
        set_free(&def);
    }

    return 1;
}

/*
Replace readable operands with their propagated copy when a mapping exists.
Params:
    - `l` - LIR block to rewrite.
    - `gen` - Map from variable/register keys to copied subjects.
    - `t` - Subject type to replace.

Returns 1 if succeeds.
*/
static int _replace_with_copy(lir_block_t* l, map_t* gen, lir_subject_type_t t) {
    iterate_ref_lir_args(lir_subject_t** curr, l, LIR_is_writeop(l->op)) {
        lir_subject_t* dst;
        if (
            (*curr)->t == t && 
            map_get(
                gen, 
                t == LIR_VARIABLE ? (*curr)->storage.var.v_id : LIR_format_register((*curr)->storage.reg.reg, 1), 
                (void**)&dst
            )
        ) {
            if ((*curr)->home == l) LIR_unload_subject(*curr);
            *curr = LIR_copy_subject(dst);
            (*curr)->home = l;
        }
    }


    return 1;
}

int LIR_variable_copy_propagation(cfg_ctx_t* cctx) {
    foreach (cfg_func_t* fb, &cctx->funcs) {
        set_t non_ssa;
        set_init(&non_ssa, SET_NO_CMP);
        map_t gen;
        map_init(&gen, MAP_NO_CMP);
        foreach (cfg_block_t* bb, &fb->blocks) {
            lir_block_t* lh = LIR_get_next(bb->lmap.entry, bb->lmap.exit, 0);
            while (lh) {
                if (!lh->unused) switch (lh->op) {
                    case LIR_TF64: case LIR_TF32:
                    case LIR_TI64: case LIR_TI32: case LIR_TI16: case LIR_TI8: 
                    case LIR_TU64: case LIR_TU32: case LIR_TU16:
                    case LIR_TU8: if (
                        lh->sarg->t != LIR_NUMBER && lh->sarg->t != LIR_CONSTVAL
                    ) break;
                    case LIR_aMOV:
                    case LIR_iMOV: {
                        if (lh->farg->t != LIR_VARIABLE) break;
                        _replace_with_copy(lh, &gen, LIR_VARIABLE);
                        if (
                            lh->op != LIR_aMOV &&
                            (
                                lh->sarg->t != LIR_VARIABLE ||
                                !set_has(&non_ssa, (void*)lh->sarg->storage.var.v_id)
                            )
                        ) map_put(&gen, lh->farg->storage.var.v_id, LIR_copy_subject(lh->sarg));
                        if (lh->op == LIR_aMOV) set_add(&non_ssa, (void*)lh->farg->storage.var.v_id);
                        break;
                    }
                    default: {
                        if (
                            LIR_is_readop(lh->op) &&
                            lh->op != LIR_aMOV &&    /* Reserved mov operations which must be saved    */
                            lh->op != LIR_REF        /* Reference demands its own independent variable */
                        ) _replace_with_copy(lh, &gen, LIR_VARIABLE);
                        break;
                    }
                }

                lh = LIR_get_next(lh, bb->lmap.exit, 1);
            }
        }

        set_free(&non_ssa);
        map_free_force_op(&gen, (int (*)(void*))LIR_unload_subject);
    }
    
    return 1;
}

int LIR_register_copy_propagation(cfg_ctx_t* cctx) {
    return 1; // TODO: noservative in base-block propagation?
}
