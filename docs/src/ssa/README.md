# SSA form
Static Single Assignment (SSA) form requires renaming all assigned variables so that each assignment creates a new, unique variable. A simple example is shown below:
![ssa](../../media/ssa_basic.png)

## Phi function
But here we encounter a problem. What should we do in this specific case?
![ssa_problem](../../media/ssa_problem.png)

Which version of the variable `a` should be used in the declaration of `b`? The answer is simple — `both`. Here’s the twist: in `SSA` form, we can use a `φ (phi)` function, which tells the compiler which variable version to use. An example of a `φ` function is shown below:
![phi_function](../../media/phi_function.png)

But how do we determine where to place this function? Here, we use previously computed dominance information. We traverse the entire symbol table of variables. For each variable, we collect the set of blocks where it is defined (either declared or assigned). Then, for each block with a definition, we take its dominance frontier blocks and insert a `φ` function there.
![phi_placement](../../media/phi_placement.png)

Entire SSA-form generator can be found [here](https://github.com/j1sk1ss/CordellCompiler/tree/HIR_LIR_SSA/src/hir/ssa). In short, implementation of this stage is next:
```c
int HIR_SSA_insert_phi(cfg_ctx_t* cctx, sym_table_t* smt) {
    list_iter_t it;
    list_iter_hinit(&cctx->funcs, &it);
    cfg_func_t* fb;
    while ((fb = (cfg_func_t*)list_iter_next(&it))) {
        HIR_CFG_compute_dom(fb);
        HIR_CFG_compute_sdom(fb);
        HIR_CFG_compute_domf(fb);
    }

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
```

Then, during the SSA renaming process, we keep track of each block that passes through a φ-function block, recording the version of the variable and the block number. This completes the SSA renaming phase, producing the following result:
![phi_final](../../media/phi_final.png)

The following code snippet from the source renames the input HIR and converts it into SSA form:
```c
switch (hh->op) {
    case HIR_PHI: {
        variable_info_t vi;
        if (VRTB_get_info_id(hh->farg->storage.var.v_id, &vi, &smt->v)) {
            varver_t* vv = _get_varver(vi.v_id, ctx);
            if (vv) {
                if (hh->sarg && vv->curr_id == hh->sarg->storage.var.v_id) break;
                int_tuple_t* inf = inttuple_create(prev_bid, vv->curr_id);
                if (!set_has_inttuple(&hh->targ->storage.set.h, inf)) set_add(&hh->targ->storage.set.h, inf);
                else inttuple_free(inf);

                int prev_id = vv->curr_id;
                int future_id = 0;
                if (hh->sarg) future_id = hh->sarg->storage.var.v_id;
                else {
                    hh->sarg = HIR_SUBJ_STKVAR(VRTB_add_copy(&vi, &smt->v), hh->farg->t, vi.s_id);
                    vv->curr_id = hh->sarg->storage.var.v_id;
                    future_id = vv->curr_id;
                }

                _insert_phi_preamble(b, prev_bid, future_id, prev_id, smt);
            }
        }

        break;
    }

    default: {
        variable_info_t vi;
        if (
            hh->farg && HIR_is_vartype(hh->farg->t) && 
            VRTB_get_info_id(hh->farg->storage.var.v_id, &vi, &smt->v) && 
            HIR_is_writeop(hh->op)
        ) {
            varver_t* vv = _get_varver(vi.v_id, ctx);
            if (vv) {
                hh->farg = HIR_SUBJ_STKVAR(VRTB_add_copy(&vi, &smt->v), hh->farg->t, vi.s_id);
                vv->curr_id = hh->farg->storage.var.v_id;
            }
        }

        _rename_block(hh, ctx);
        break;
    }
}
```

# Liveness analyzer part
Several optimization techniques are based on data-flow analysis. Data-flow analysis itself relies on liveness analysis, which in turn depends on the program’s `SSA` form and control-flow graph (CFG). Now that we have established these fundamental representations, we can proceed with the `USE–DEF–IN–OUT` computation process.

## USE and DEF
`USE` and `DEF` are two sets associated with every `CFG` block. These sets represent all definitions and usages of variables within the block (recall that the code is already in `SSA` form). In short:
- `DEF` contains all variables that are written (i.e., assigned a new value).
- `USE` contains all variables that are read (i.e., their value is used).
![use_def](../../media/use_def.png)

Implementation of this part is simple (source code is [here](https://github.com/j1sk1ss/CordellCompiler/tree/HIR_LIR_SSA/src/hir/dfg)):
```c
int LIR_DFG_collect_defs(cfg_ctx_t* cctx) {
    list_iter_t fit;
    list_iter_hinit(&cctx->funcs, &fit);
    cfg_func_t* fb;
    while ((fb = (cfg_func_t*)list_iter_next(&fit))) {
        list_iter_t bit;
        list_iter_hinit(&fb->blocks, &bit);
        cfg_block_t* cb;
        while ((cb = (cfg_block_t*)list_iter_next(&bit))) {
            set_init(&cb->def);
            hir_block_t* hh = cb->entry;
            while (hh) {
                if (!hh->unused) {
                    if (hh->op == HIR_PHI && HIR_is_vartype(hh->sarg->t)) set_add(&cb->def, (void*)hh->sarg->storage.var.v_id);
                    else if (HIR_is_writeop(hh->op) && HIR_is_vartype(hh->farg->t)) set_add(&cb->def, (void*)hh->farg->storage.var.v_id);
                }

                if (hh == cb->exit) break;
                hh = hh->next;
            }
        }
    }

    return 1;
}

int LIR_DFG_collect_uses(cfg_ctx_t* cctx) {
    list_iter_t fit;
    list_iter_hinit(&cctx->funcs, &fit);
    cfg_func_t* fb;
    while ((fb = (cfg_func_t*)list_iter_next(&fit))) {
        list_iter_t bit;
        list_iter_hinit(&fb->blocks, &bit);
        cfg_block_t* cb;
        while ((cb = (cfg_block_t*)list_iter_next(&bit))) {
            set_init(&cb->use);
            hir_block_t* hh = cb->entry;
            while (hh) {
                if (!hh->unused) {
                    if (hh->op != HIR_PHI) {
                        hir_subject_t* args[3] = { hh->farg, hh->sarg, hh->targ };
                        for (int i = HIR_is_writeop(hh->op); i < 3; i++) {
                            if (args[i] && HIR_is_vartype(args[i]->t)) {
                                set_add(&cb->use, (void*)args[i]->storage.var.v_id);
                            }
                        }
                    }
                    else {
                        set_iter_t it;
                        set_iter_init(&hh->targ->storage.set.h, &it);
                        int_tuple_t* tpl;
                        while (set_iter_next(&it, (void**)&tpl)) {
                            set_add(&cb->use, (void*)tpl->y);
                        }
                    }
                }

                if (hh == cb->exit) break;
                hh = hh->next;
            }
        }
    }

    return 1;
}
```

## IN and OUT
`IN` and `OUT` is a little bit complex part here. 
```
OUT[B] = union(IN[S])
IN[B]  = union(USE[B], OUT[B] − DEF[B])
```

Implementation of this formula is below:
```c
static int _compute_out(cfg_block_t* cfg) {
    set_t out;
    set_init(&out);
    if (cfg->l)   set_union(&out, &out, &cfg->l->curr_in);
    if (cfg->jmp) set_union(&out, &out, &cfg->jmp->curr_in);
    set_free(&cfg->curr_out);
    set_copy(&cfg->curr_out, &out);
    set_free(&out);
    return 1;
}

static int _compute_in(cfg_block_t* cfg) {
    set_t tmp;
    set_init(&tmp);
    set_copy(&tmp, &cfg->curr_out);
    set_minus_set(&tmp, &cfg->def);
    set_free(&cfg->curr_in);
    set_union(&cfg->curr_in, &cfg->use, &tmp);
    set_free(&tmp);
    return 1;
}
```

First of all, to make the calculation much faster, we should traverse our list of `CFG` blocks in reverse order, computing `IN` and `OUT` for each block using the formulas above, and repeat this process until it stabilizes. Stabilization occurs when the previous sets (`primeIN` and `primeOUT`) are equal to the current sets (`currIN` and `currOUT`). This means that for every block we should maintain four sets:

- primeIN
- currIN
- primeOUT
- currOUT

After each iteration, the current values are copied into the corresponding prime sets, preparing them for the next comparison cycle.
![in_out](../../media/in_out.png)

And this is how this code calculate `IN` and `OUT`:
```c
int LIR_DFG_compute_inout(cfg_ctx_t* cctx) {
    list_iter_t fit;
    list_iter_hinit(&cctx->funcs, &fit);
    cfg_func_t* fb;
    while ((fb = (cfg_func_t*)list_iter_next(&fit))) {
        while (1) {
            list_iter_t bit;
            list_iter_tinit(&fb->blocks, &bit);
            cfg_block_t* cb;
            while ((cb = (cfg_block_t*)list_iter_prev(&bit))) {
                _compute_out(cb);
                _compute_in(cb);
            }

            int same = 1;
            list_iter_tinit(&fb->blocks, &bit);
            while ((cb = (cfg_block_t*)list_iter_prev(&bit))) {
                if (!set_equal(&cb->curr_in, &cb->prev_in) || !set_equal(&cb->curr_out, &cb->prev_out)) {
                    same = 0;
                    break;
                }
            }

            if (same) break;
            list_iter_tinit(&fb->blocks, &bit);
            while ((cb = (cfg_block_t*)list_iter_prev(&bit))) {
                set_free(&cb->prev_in);
                set_copy(&cb->prev_in, &cb->curr_in);
                set_free(&cb->prev_out);
                set_copy(&cb->prev_out, &cb->curr_out);
            }
        }
    }

    return 1;
}
```

## Point of deallocation
At this point, we can determine where each variable dies. If a variable appears in the `IN` or `DEF` set but is not present in the `OUT` set, it means the variable is no longer used after this block, and we can safely insert a special `kill` instruction to mark it as dead. However, an important detail arises when dealing with pointer types. To handle them correctly, we construct a special structure called an `aliasmap`, which tracks ownership relationships between variables. This map records which variable owns another — meaning that one variable’s lifetime depends on another’s. For example, in code like this:
```cpl
{
   i32 a0 = 10;
   ptr i32 b0 = ref a0;
   dref b0 = 20;
}
```
the variable `a` is owned by `b`, so we must not kill `a` while `b` is still alive. In other words, the liveness of `a` depends on the liveness of `b`, and this dependency is preserved through the aliasmap.
![kill_var](../../media/kill_var.png)

This compiler implements this logic in next way (source code is [here](https://github.com/j1sk1ss/CordellCompiler/tree/HIR_LIR_SSA/src/hir/dfg)).
```c
int LIR_DFG_create_deall(cfg_ctx_t* cctx, sym_table_t* smt) {
    list_iter_t fit;
    list_iter_hinit(&cctx->funcs, &fit);
    cfg_func_t* fb;
    while ((fb = (cfg_func_t*)list_iter_next(&fit))) {
        list_iter_t bit;
        list_iter_hinit(&fb->blocks, &bit);
        cfg_block_t* cb;
        while ((cb = (cfg_block_t*)list_iter_next(&bit))) {
            set_t appeared;
            set_init(&appeared);
            set_union(&appeared, &cb->curr_in, &cb->def);

            set_iter_t init;
            set_iter_init(&appeared, &init);
            long vid;
            while (set_iter_next(&init, (void**)&vid)) {
                if (set_has(&cb->curr_out, (void*)vid)) continue;

                set_t owners;
                int hasown = 0;
                if (ALLIAS_get_owners(vid, &owners, &smt->m)) {
                    set_iter_t ownersit;
                    set_iter_init(&owners, &ownersit);
                    while (set_iter_next(&ownersit, (void**)&vid)) {
                        if (set_has(&cb->curr_out, (void*)vid)) {
                            hasown = 1;
                            break;
                        }
                    }
                }

                set_free_force(&owners);
                if (hasown) continue;
                HIR_insert_block_after(HIR_create_block(HIR_VRDEALL, HIR_SUBJ_CONST(vid), NULL, NULL), cb->exit);

                map_iter_t mit;
                map_iter_init(&smt->m.allias, &mit);
                allias_t* al;
                while (map_iter_next(&mit, (void**)&al)) {
                    if (!set_has(&al->owners, (void*)vid)) continue;
                    if (ALLIAS_mark_owner(al->v_id, vid, &smt->m)) {
                        HIR_insert_block_after(HIR_create_block(HIR_VRDEALL, HIR_SUBJ_CONST(al->v_id), NULL, NULL), cb->exit);
                        set_free_force(&al->delown);
                        set_init(&al->delown);
                    }

                    break;
                }
            }

            set_free(&appeared);
        }
    }
}
```

## Example code
```cpl
{
    start(i64 argc, ptr u64 argv) {
        i32 a = 10;
        ptr i32 b = ref a;
        dref b = 11;
        i32 c = 10;
        exit c;
    }
}
```
->
```
{
    start {
        alloc i64s argc0;
        kill c0
        load_starg(i64s argc9);
        kill c9
        alloc u64s argv1;
        kill c1
        load_starg(u64s argv10);
        kill c10
        {
            alloc i32s a2;
            kill c2
            i32t tmp5 = n10 as i32;
            i32s a11 = i32t tmp5;
            kill c5
            alloc u64s b3;
            kill c3
            u64t tmp6 = &(i32s a11);
            u64s b12 = u64t tmp6;
            kill c6
            u64t tmp7 = n11 as u64;
            *(u64s b12) = u64t tmp7;
            kill c11
            kill c12
            kill c7
            alloc i32s c4;
            kill c4
            i32t tmp8 = n10 as i32;
            i32s c13 = i32t tmp8;
            kill c8
            exit i32s c13;
            kill c13
        }
    }
}
```