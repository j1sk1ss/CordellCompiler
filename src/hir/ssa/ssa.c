/* ssa.c - Convert the input HIR of the program (with PHI placeholders) to the SSA form */
#include <hir/ssa.h>

typedef struct {
    long curr_id;
    long v_id;
} varver_t;

/*
Add variable version placeholder to the versions map.
Params:
    - `vers` - Versions map.
    - `id` - Current variable ID (Or the head variable ID).
    - `cid` - Current variable ID.

Returns 1 if all succeed. Otherwise will return 0.
*/
static int _add_varver(map_t* vers, long id, long cid) {
    varver_t* vv = (varver_t*)mm_malloc(sizeof(varver_t));
    if (!vv) return 0;
    vv->v_id    = id;
    vv->curr_id = cid;
    return map_put(vers, id, (void*)vv);
}

/*
Retrieve bariable information placeholder from the versions map.
Params:
    - `v_id` - Variable 'base' ID.
    - `ctx` - SSA context.

Returns variable information or NULL.
*/
static inline varver_t* _get_varver(long v_id, ssa_ctx_t* ctx) {
    varver_t* vi;
    if (map_get(&ctx->vers, v_id, (void**)&vi)) return vi;
    return NULL;
}

/*
Rename the provided HIR block with current SSA context.
Params:
    - `h` - HIR block for rename.
    - `ctx` - SSA context.

Returns 1 if all succeed. Otherwise will return 0.
*/
static int _rename_block(hir_block_t* h, ssa_ctx_t* ctx) {
    hir_subject_t* args[3] = { h->farg, h->sarg, h->targ };
    for (int i = HIR_writeop(h->op); i < 3; i++) {
        if (!args[i]) continue;
        if (HIR_is_vartype(args[i]->t)) {
            varver_t* vv = _get_varver(args[i]->storage.var.v_id, ctx);
            if (vv) args[i]->storage.var.v_id = vv->curr_id;
        }
        else if (args[i]->t == HIR_ARGLIST) {
            foreach (hir_subject_t* s, &args[i]->storage.list.h) {
                if (!HIR_is_vartype(s->t)) continue;
                varver_t* vv = _get_varver(s->storage.var.v_id, ctx);
                if (vv) s->storage.var.v_id = vv->curr_id;
            }
        }
    }

    return 1;
}

/*
Insert PHI preambule to the previous block.
The idea is simple: We need to assist the compiler with future SSA form destruction. 
To perform this, we can mark for the compiler, how variables are linked with each other.
For instance:
```cpl
    function foo() => i32 {
    : BB1 :
        i32 a_1 = 0;
        if 1; {
    : BB2 :
            a_2 = 1;
        }
        else {
    : BB3 :
            a_3 = 2;
        }
    : BB4 :
    :   a_4 = phi(a_3, a_2); :
        return a_4;
    }
```

SSA form will put the phi function before BB4 block, but when we will start code generation, this will interrupt us.
To prevent this, we can append a hidden command such a 'HIR_PREAMBULE' that will work similar to 'LIR_MOVE':
```cpl
    function foo() => i32 {
    : BB1 :
        i32 a_1 = 0;
        if 1; {
    : BB2 :
            a_2 = 1;
    :       a_4 = a_2; :
        }
        else {
    : BB3 :
            a_3 = 2;
    :       a_4 = a_3; :
        }
    : BB4 :
    :   a_4 = phi(a_3, a_2); :
        return a_4;
    }
```

Params:
    - `block` - Current CFG Basic Block.
    - `bid` - Target previous Basic Block.
    - `a` - Future variable ID.
    - `b` - Previous variable ID.
    - `smt` - Symtable.

Returns 1 if all succeed. Otherwise will return 0.
*/
static int _insert_phi_preamble(cfg_block_t* block, long bid, int a, int b, sym_table_t* smt) {
    if (a == b) return 1;     /* Check is this isn't the same variables */
    variable_info_t avi, bvi; /* Check is these variables are existing  */
    if (
        !VRTB_get_info_id(a, &avi, &smt->v) || 
        !VRTB_get_info_id(b, &bvi, &smt->v)
    ) return 0;

    set_foreach (cfg_block_t* trg, &block->pred) {
        if (trg->id != bid) continue;
        hir_block_t* union_command = HIR_create_block(
            HIR_PHI_PREAMBLE, 
            HIR_SUBJ_STKVAR(avi.v_id, HIR_get_stktype(&avi)), 
            HIR_SUBJ_STKVAR(bvi.v_id, HIR_get_stktype(&bvi)), 
            NULL
        );

        if (trg->hmap.exit) HIR_insert_block_before(union_command, trg->hmap.exit);
        else {
            HIR_CFG_append_hir_block_back(trg, union_command);
            HIR_insert_block_before(union_command, trg->l->hmap.entry);
        }

        if (trg->hmap.entry == trg->hmap.exit) {
            trg->hmap.entry = union_command;
        }
        
        break;
    }

    return 1;
}

/*
Base routine for block SSA processing.
Params:
    - `b` - Current BaseBlock.
    - `ctx` - SSA context.
    - `prev_bid` - [Service information] For inital value use '-1'.
    - `smt` - Symtable.

Returns 1 if succeed. Otherwise will return 0.
*/
static int _iterate_block(cfg_block_t* b, ssa_ctx_t* ctx, long prev_bid, sym_table_t* smt) {
    if (!b || set_has(&b->visitors, (void*)prev_bid)) return 0;

    hir_block_t* hh = b->hmap.entry;
    while (hh) {
        switch (hh->op) {
            /* Special PHI function logic implies handling the phi set in command.
            In other words, we take a new variable ID from the VRTB_add_copy function (If it doesn't exist).
            Then we append the current variable ID to this list */
            case HIR_PHI: {
                variable_info_t vi; /* Get base variable ID.                                      */
                                    /* Note: The base variable ID is placed in the first argument */
                if (VRTB_get_info_id(hh->farg->storage.var.v_id, &vi, &smt->v)) {
                    varver_t* vv = _get_varver(vi.v_id, ctx);
                    if (vv) {
                        /* If we are working with the same variable, that is already existes */
                        /* we can safely skip it                                             */
                        if (hh->sarg && vv->curr_id == hh->sarg->storage.var.v_id) break;

                        /* Create information for PHI function             */
                        /* This code will create tuple with a BB<->ID data */
                        int_tuple_t* inf = inttuple_create(prev_bid, vv->curr_id);
                        if (!set_has_inttuple(&hh->targ->storage.set.h, inf)) set_add(&hh->targ->storage.set.h, inf);
                        else inttuple_free(inf);

                        int future_id = 0;
                        int prev_id = vv->curr_id;
                        if (hh->sarg) future_id = hh->sarg->storage.var.v_id; /* Skip new variable creation                        */
                                                                              /* If new variable is existes, that means we already */
                                                                              /* rename all blocks below us                        */
                        else {
                            hh->sarg = HIR_SUBJ_STKVAR(VRTB_add_copy(&vi, &smt->v), hh->farg->t);
                            vv->curr_id = hh->sarg->storage.var.v_id;
                            future_id = vv->curr_id;
                        }

                        _insert_phi_preamble(b, prev_bid, future_id, prev_id, smt);
                    }
                }

                break;
            }

            default: {
                _rename_block(hh, ctx);
                
                variable_info_t vi;
                if (
                    hh->farg && HIR_is_vartype(hh->farg->t) &&                    /* - If this command has variable as the first arg  */
                    !HIR_is_tmptype(hh->farg->t) &&                               /* - This compiler initialy creates tmp variables   */
                                                                                  /* as SSA variables                                 */
                    VRTB_get_info_id(hh->farg->storage.var.v_id, &vi, &smt->v) && /* - If this variable existes in the var symtab     */
                    HIR_writeop(hh->op)                                           /* - If this command assignes new value to variable */
                ) {
                    varver_t* vv = _get_varver(vi.v_id, ctx);
                    if (vv) {
                        hir_subject_type_t tmp_t = hh->farg->t;
                        if (hh->farg->home == hh) HIR_unload_subject(hh->farg); 
                        hh->farg = HIR_SUBJ_STKVAR(VRTB_add_copy(&vi, &smt->v), tmp_t);

                        array_info_t ai; /* Check if this variable is an array                      */
                                         /* Note: This is necessary due to possible function inline */
                                         /*       In the nutshell, we must be sure that copied func */
                                         /*       will have unique arrays.                          */
                        if (ARTB_get_info(vi.v_id, &ai, &smt->a)) {
                            ARTB_add_copy(hh->farg->storage.var.v_id, &ai, &smt->a);
                        }

                        vv->curr_id = hh->farg->storage.var.v_id;
                    }
                }

                break;
            }
        }

        if (hh == b->hmap.exit) break;
        hh = hh->next;
    }

    set_add(&b->visitors, (void*)prev_bid);

    if (!b->jmp || !b->l) {
        _iterate_block(b->jmp, ctx, b->id, smt);
        _iterate_block(b->l, ctx, b->id, smt);
    }
    else {
        map_t saved;
        map_init(&saved, MAP_NO_CMP);
        map_foreach (varver_t* s, &ctx->vers) {
            _add_varver(&saved, s->v_id, s->curr_id);
        }

        _iterate_block(b->jmp, ctx, b->id, smt);
        map_free_force(&ctx->vers);
        map_init(&ctx->vers, MAP_NO_CMP);
        
        map_foreach (varver_t* s, &saved) {
            _add_varver(&ctx->vers, s->v_id, s->curr_id);
        }
        
        _iterate_block(b->l, ctx, b->id, smt);
        map_free_force(&saved);
    }

    return 1;
}

int HIR_SSA_rename(cfg_ctx_t* cctx, ssa_ctx_t* ctx, sym_table_t* smt) {
    /* Build initial map of existed variables in the program   */
    /* Important note: We don't care about tmp variables.      */
    /*                 We can do it considering the mechanism  */
    /*                 of tmp variable generation (see hirgen) */
    map_foreach (variable_info_t* vh, &smt->v.vartb) {
        if (vh->vfs.ro || TKN_istmp_type(vh->type)) continue;
        _add_varver(&ctx->vers, vh->v_id, vh->v_id);
    }

    /* Go to each function and perform SSA convert operation */
    foreach (cfg_func_t* fb, &cctx->funcs) {
        if (!fb->used) continue;
        _iterate_block(list_get_head(&fb->blocks), ctx, 0, smt);
    }

    map_free_force(&ctx->vers);
    return 1;
}
