#include <hir/constfold.h>

/*
Register a new parameters list for a function. We need to be sure, that
all function calls have the same list of arguments.
Params:
    - `f_id` - Function's ID.
    - `args` - Input parameters.
    - `fcalls` - FUnction calls map.

Returns 1 if registration was success, otherwise it returs 0.
*/
static int _register_params(symbol_id_t f_id, list_t* args, map_t* fcalls) {
    list_t* rargs;
    if (map_get(fcalls, f_id, (void**)&rargs)) {
        void** prev = list_flatten(rargs);
        void** curr = list_flatten(args);

        for (int i = 0; i < list_size(args); i++) {
            if (
                prev[i] && curr[i] &&
                HIR_hash_subject((hir_subject_t*)prev[i]) != HIR_hash_subject((hir_subject_t*)curr[i])
            ) list_replace(rargs, prev[i], NULL);
        }

        mm_free(prev);
        mm_free(curr);
    }
    else {
        rargs = (list_t*)mm_malloc(sizeof(list_t));
        if (!rargs) return 0;
        list_init(rargs);
        list_copy(args, rargs);
        map_put(fcalls, f_id, rargs);
    }

    return 1;
}

/*
Register a new function call in function call map.
Params:
    - `cctx` - CFG context.
    - `fcalls` - Function call map.

Returns 1 if succeeds.
*/
static int _register_fcalls(cfg_ctx_t* cctx, map_t* fcalls) {
    foreach (cfg_func_t* fb, &cctx->funcs) {
        foreach (cfg_block_t* bb, &fb->blocks) {
            hir_block_t* hh = HIR_get_next(bb->hmap.entry, bb->hmap.exit, 0);
            while (hh) {
                switch (hh->op) {
                    case HIR_FCLL:
                    case HIR_STORE_FCLL: _register_params(hh->sarg->storage.str.s_id, &hh->targ->storage.list.h, fcalls); break;
                    default: break;
                }

                hh = HIR_get_next(hh, bb->hmap.exit, 1);
            }
        }
    }

    return 1;
}

/*
Extract defined value form a subject.
Params:
    - `s` - Subject for extraction.
    - `smt` - Symtable.
    - `val` - Output location.

Returns 1 if succeeds, otherwise will return 0.
*/
static int _extract_variable_value(hir_subject_t* s, sym_table_t* smt, long* val) {
    switch (HIR_is_defined_type(s->t)) {
        case 1: *val = s->storage.num.value->to_llong(s->storage.num.value); return 1;
        case 2: *val = s->storage.cnst.value;                                return 1;
        default: break;
    }

    if (HIR_is_vartype(s->t)) {
        variable_info_t vi;
        if (VRTB_get_info_id(s->storage.var.v_id, &vi, &smt->v) && vi.vdi.defined == DEFINED_VARIABLE) {
            *val = vi.vdi.definition;
            return 1;
        }
    }

    return 0;
}

/*
Propagate parameters and replace the load arguments with the defined value.
Params:
    - `cctx` - CFG context.
    - `smt` - Symtable.
    - `fcalls` - Function call map.

Returns 1 if the function has propagated something, otherwise will return 0.
*/
static int _propagate_params(cfg_ctx_t* cctx, sym_table_t* smt, map_t* fcalls) {
    int changed = 0;
    foreach (cfg_func_t* fb, &cctx->funcs) {
        list_t* rargs;
        if (!map_get(fcalls, fb->f_id, (void**)&rargs)) continue;

        int param_index = 0;
        void** registered_params = list_flatten(rargs);
        foreach (cfg_block_t* bb, &fb->blocks) {
            hir_block_t* hh = HIR_get_next(bb->hmap.entry, bb->hmap.exit, 0);
            while (hh) {
                if (hh->op == HIR_FARGLD) {
                    long value = 0;
                    hir_subject_t* folded = (hir_subject_t*)registered_params[param_index++];
                    if (folded && _extract_variable_value(folded, smt, &value)) {
                        hh->op = HIR_STORE;
                        HIR_unload_subject(hh->sarg);
                        hh->sarg = HIR_SUBJ_CONST(value);
                        VRTB_update_definition(hh->farg->storage.var.v_id, value, NO_SYMBOL_ID, &smt->v, 0);
                        changed = 1;
                    }
                }

                hh = HIR_get_next(hh, bb->hmap.exit, 1);
            }
        }

        mm_free(registered_params);
    }

    return changed;
}

/*
Unload function call map.
Params:
    - `l` - Entry list.

Returns 1 if succeeds.
*/
static int _unload_fcalls(list_t* l) {
    list_free(l);
    return mm_free(l);
}

int HIR_sparse_const_funcall_propagation(cfg_ctx_t* cctx, sym_table_t* smt) {
    map_t fcalls;
    map_init(&fcalls, MAP_NO_CMP);

    _register_fcalls(cctx, &fcalls);
    int res = _propagate_params(cctx, smt, &fcalls);

    map_free_force_op(&fcalls, (int (*)(void*))_unload_fcalls);
    return res;
}

/*
Collect and register function's return if the function has single return,
and it has a return in general.
Params:
    - `cctx` - CFG context.
    - `frets` - Function returns map.

Returns 1 if succeeds.
*/
static int _collect_frets(cfg_ctx_t* cctx, map_t* frets) {
    foreach (cfg_func_t* fb, &cctx->funcs) {
        list_t ret_blocks;
        list_init(&ret_blocks);

        int entry = 1;
        foreach (cfg_block_t* bb, &fb->blocks) {
            if (!bb->l && !bb->jmp && (set_size(&bb->pred) || entry)) {
                list_add(&ret_blocks, bb);
            }

            entry = 0;
        }
        
        if (list_size(&ret_blocks) == 1) {
            cfg_block_t* rblock = (cfg_block_t*)ret_blocks.h->data; 
            if (rblock->hmap.exit->op == HIR_FRET) { 
                map_put(frets, fb->f_id, rblock->hmap.exit->farg);
            }
        }

        list_free(&ret_blocks);
    }

    return 1;
}

/*
If there is a defined return, we can replace a function call of the function
with its defined return.
Params:
    - `cctx` - CFG context.
    - `smt` - Symtable.
    - `frets` - Function returns map.

Returns 1 if it has changes something, otherwise will return 0.
*/
static int _propagate_frets(cfg_ctx_t* cctx, sym_table_t* smt, map_t* frets) {
    int changed = 0;
    foreach (cfg_func_t* fb, &cctx->funcs) {
        foreach (cfg_block_t* bb, &fb->blocks) {
            hir_block_t* hh = HIR_get_next(bb->hmap.entry, bb->hmap.exit, 0);
            while (hh) {
                if (HIR_is_ret_funccall(hh->op)) {
                    hir_subject_t* ret;
                    if (
                        map_get(frets, hh->sarg->storage.str.s_id, (void**)&ret) && 
                        (HIR_is_vartype(ret->t) || HIR_is_defined_type(ret->t))
                    ) { 
                        long value = 0;
                        if (_extract_variable_value(ret, smt, &value)) {
                            changed = VRTB_update_definition(hh->farg->storage.var.v_id, value, NO_SYMBOL_ID, &smt->v, 0) || changed;
                        }
                    }
                }

                hh = HIR_get_next(hh, bb->hmap.exit, 1);
            }
        }
    }

    return changed;
}

int HIR_sparce_const_fret_propagation(cfg_ctx_t* cctx, sym_table_t* smt) {
    map_t frets;
    map_init(&frets, MAP_NO_CMP);

    _collect_frets(cctx, &frets);
    int res = _propagate_frets(cctx, smt, &frets);

    map_free(&frets);
    return res;
}
