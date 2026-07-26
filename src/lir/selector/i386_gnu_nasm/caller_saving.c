#include <lir/selector/i386_gnu_nasm.h>

/* Collect used registers in the provided function.
Params:
    - `dirty` - Output set of used registers.
    - `f` - Function CFG.
            Note: If this value is NULL, will set all registers as
                  dirty.

Returns 1 on success, otherwise 0. */
static int _collect_in_function_reg_usage(set_t* dirty, cfg_func_t* f, symbol_id_t f_id, sym_table_t* smt) {
    if (!f) {
        func_info_t fi;
        if (FNTB_get_info_id(f_id, &fi, &smt->f)) {
            // if (fi.flags.external && fi.flags.abi) {
            //     lir_registers_t dirty_regs[] = { EAX, ECX, EDX };
            //     for (int i = 0; i < (int)(sizeof(dirty_regs) / sizeof(RBX)); i++) {
            //         set_add(dirty, (void*)dirty_regs[i]);
            //     }
            // } TODO
            // else {
                lir_registers_t dirty_regs[] = { EBX, ECX, EDX, ESI, EDI, EBP };
                for (int i = 0; i < (int)(sizeof(dirty_regs) / sizeof(dirty_regs[0])); i++) {
                    set_add(dirty, (void*)dirty_regs[i]);
                }
            // }
        }

        return 1;
    }
    else {
        foreach (cfg_block_t* bb, &f->blocks) {
            iterate_lir_instructions (bb) {
                if (
                    LIR_is_writeop(lh->op) &&   /* We are writing some value to register (for some reason)         */
                    lh->farg->t == LIR_REGISTER /* This is a register object, we can say that this is a dirty one. */
                ) set_add(dirty, (void*)LIR_format_register(lh->farg->storage.reg.reg, 4));
            }
        }
    }

    return 1;
}

static unsigned long long _visit_counter = 0;

/* Collect register usage in the further CFG.
Params:
    - `dirty` - Previously rewritten registers.
    - `save` - Output set.
    - `bbh` - Current BasicBlock.
    - `off` - Lir block off.

Returns 1 on success, otherwise 0. */
static int _collect_out_function_reg_usage(set_t* dirty, set_t* save, cfg_block_t* bbh, lir_block_t* off) {
    if (!bbh || !set_size(dirty)) return 0;
    if (bbh->visited != _visit_counter) bbh->visited = _visit_counter;
    else return 0;
    
    lir_block_t* lh = off ? off : bbh->lmap.entry;
    while (lh) {
        if ( /* Remove register from the 'dirty' set if it is rewritten */
            LIR_is_writeop(lh->op) && 
            lh->farg->t == LIR_REGISTER &&
            !LIR_subj_equals(lh->farg, lh->sarg)
        ) set_remove(dirty, (void*)LIR_format_register(lh->farg->storage.reg.reg, 4));
        
        iterate_lir_args (lir_subject_t* arg, lh, LIR_is_writeop(lh->op)) {
            if (
                arg->t != LIR_REGISTER || 
                !set_has(dirty, (void*)LIR_format_register(arg->storage.reg.reg, 4))
            ) continue; /* If this register isn't a dirty one -> skip it */
            set_add(save, (void*)LIR_format_register(arg->storage.reg.reg, 4));
        }
        
        lh = LIR_get_next(lh, bbh->lmap.exit, 1);
    }

    set_t copy;

    set_copy(&copy, dirty);
    _collect_out_function_reg_usage(&copy, save, bbh->l, off);
    set_free(&copy);
    
    set_copy(&copy, dirty);
    _collect_out_function_reg_usage(&copy, save, bbh->jmp, off);
    set_free(&copy);

    return 0;
}

static inline lir_block_t* _find_pre_argload(lir_block_t* lh, lir_block_t* ex) {
    lir_subject_t* last = NULL;
    while (lh && lh != ex) {
        if (lh->op == LIR_PUSH) {
            last = lh->farg;
            goto _next_inst;
        }
        if (last && LIR_is_movop(lh->op) && LIR_subj_equals(last, lh->farg)) {
            last = NULL;
            goto _next_inst;
        }
        return lh;
_next_inst: {}
        lh = lh->prev;
    }

    return NULL;
}

static inline lir_block_t* _find_post_argunload(lir_block_t* lh, lir_block_t* ex) {
    while (lh && lh != ex) {
        if (lh->op != LIR_POP && lh->op != LIR_iADD) return lh;
        lh = lh->next;
    }

    return NULL;
}

int i386_gnu_nasm_caller_saving(cfg_ctx_t* cctx, call_graph_t* calls, sym_table_t* smt) {
    foreach (cfg_func_t* fb, &cctx->funcs) {
        if (!fb->used) continue;
        foreach (cfg_block_t* bb, &fb->blocks) {
            iterate_lir_instructions (bb) {
                switch (lh->op) {
                    case LIR_FCLL: {
                        set_t func_regs, save_regs;
                        set_init(&func_regs, SET_NO_CMP);
                        set_init(&save_regs, SET_NO_CMP);
                        
                        _visit_counter = CFG_get_unique_counter();

                        cfg_func_t* func = NULL;
                        symbol_id_t f_id = NO_SYMBOL_ID;

                        if (lh->farg->t == LIR_FNAME) {
                            f_id = lh->farg->storage.str.sid;
                            map_get(&cctx->fmap, lh->farg->storage.str.sid, (void**)&func);
                        }
                        else if (lh->farg->t == LIR_VARIABLE) {
                            f_id = lh->farg->storage.var.v_id;
                            set_t funcs;
                            ALLIAS_get_slaves(lh->farg->storage.var.v_id, &funcs, &smt->m);
                            set_foreach (symbol_id_t slave_id, &funcs) {
                                f_id = slave_id;
                                if (map_get(&cctx->fmap, slave_id, (void**)&func) && func) break;
                            }

                            set_free(&funcs);
                        }

                        queue_t work_list;
                        queue_init(&work_list);
                        queue_push(&work_list, func);

                        while (queue_pop(&work_list, (void**)&func)) {
                            _collect_in_function_reg_usage(&func_regs, func, f_id, smt);
                            if (!func) continue;
                            call_graph_node_t* call;
                            if (map_get(&calls->verts, func->f_id, (void**)&call)) {
                                set_foreach(call_graph_node_t* f, &call->edges) {
                                    cfg_func_t* another = NULL;
                                    if (f == call) continue;
                                    if (map_get(&cctx->fmap, f->f_id, (void**)&another)) queue_push(&work_list, another);
                                    else queue_push(&work_list, NULL);
                                }
                            }
                        }

                        _collect_out_function_reg_usage(&func_regs, &save_regs, bb, lh->next);
                        queue_free(&work_list);

                        set_foreach (long reg, &save_regs) {
                            if (reg == EAX) continue;
                            LIR_insert_block_after(LIR_create_block(LIR_PUSH, LIR_SUBJ_REG(reg, 4), NULL, NULL), _find_pre_argload(lh->prev, bb->lmap.exit));
                            LIR_insert_block_before(LIR_create_block(LIR_POP, LIR_SUBJ_REG(reg, 4), NULL, NULL), _find_post_argunload(lh->next, bb->lmap.exit));
                        }
                        
                        set_free(&func_regs);
                        set_free(&save_regs);
                        break;
                    }
                    default: break;
                }
            }
        }
    }

    return 1;
}
