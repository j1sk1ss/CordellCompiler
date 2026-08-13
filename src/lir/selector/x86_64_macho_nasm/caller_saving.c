#include <lir/selector/x86_64_macho_nasm.h>
 // TODO: If this an ABI function, then don't save all regs, save just caller save
/* Collect used registers in the provided function.
Params:
    - `dirty` - Output set of used registers.
    - `f` - Function CFG.
            Note: If this value is NULL, will set all registers as
                  dirty.

Returns 1 on success, otherwise 0 */
static void _collect_in_function_reg_usage(set_t* dirty, cfg_func_t* f, symbol_id_t f_id, sym_table_t* smt) {
    if (!f) {
        func_info_t fi;
        if (FNTB_get_info_id(f_id, &fi, &smt->f) && fi.flags.abi) {
            lir_registers_t dirty_regs[] = { RAX, RCX, RDX, RSI, RDI, R8, R9, R10, R11 };
            for (int i = 0; i < (int)(sizeof(dirty_regs) / sizeof(dirty_regs[0])); i++) {
                set_add(dirty, (void*)dirty_regs[i]);
            }
        }
        else {
_unknown_call: {}
            lir_registers_t dirty_regs[] = { RBX, RCX, RDX, RSI, RDI, RBP, R8, R9, R10, R11, R12, R13, R14, R15 };
            for (int i = 0; i < (int)(sizeof(dirty_regs) / sizeof(dirty_regs[0])); i++) {
                set_add(dirty, (void*)dirty_regs[i]);
            }
        }
    }
    else {
        foreach (cfg_block_t* bb, &f->blocks) {
            iterate_lir_instructions (bb) {
                if (
                    (lh->op == LIR_FCLL || lh->op == LIR_ECLL) &&
                    lh->farg->t != LIR_FNAME
                ) goto _unknown_call;
                if (
                    LIR_is_writeop(lh->op) &&   /* We are writing some value to register (for some reason)        */
                    lh->farg->t == LIR_REGISTER /* This is a register object, we can say that this is a dirty one */
                ) set_add(dirty, (void*)LIR_format_register(lh->farg->storage.reg.reg, CONF_get_full_bytness()));
            }
        }
    }
}

static unsigned long long _visit_counter = 0;

static int _collect_out_function_reg_usage(set_t* dirty, set_t* save, cfg_block_t* bbh, lir_block_t* off);

static void _collect_local_out_function_reg_usage(set_t* dirty, set_t* save, cfg_block_t* bbh, lir_block_t* off) {
    if (!bbh || !set_size(dirty)) return;
    lir_block_t* lh = off ? off : bbh->lmap.entry;
    while (lh) {
        if (
            LIR_is_writeop(lh->op) &&
            lh->farg->t == LIR_REGISTER &&
            !LIR_subj_equals(lh->farg, lh->sarg)
        ) set_remove(dirty, (void*)LIR_format_register(lh->farg->storage.reg.reg, CONF_get_full_bytness()));

        iterate_lir_args (lir_subject_t* arg, lh, LIR_is_writeop(lh->op)) {
            if (
                arg->t != LIR_REGISTER ||
                !set_has(dirty, (void*)LIR_format_register(arg->storage.reg.reg, CONF_get_full_bytness()))
            ) continue;
            set_add(save, (void*)LIR_format_register(arg->storage.reg.reg, CONF_get_full_bytness()));
        }

        if (lh->op == LIR_JMP) {
            _collect_out_function_reg_usage(dirty, save, bbh->jmp, NULL);
            return;
        }

        lh = LIR_get_next(lh, bbh->lmap.exit, 1);
    }
}

/* Collect register usage in the further CFG.
Params:
    - `dirty` - Previously rewritten registers.
    - `save` - Output set.
    - `bbh` - Current BasicBlock.
    - `off` - Lir block off.

Returns 1 on success, otherwise 0 */
static int _collect_out_function_reg_usage(set_t* dirty, set_t* save, cfg_block_t* bbh, lir_block_t* off) {
    if (!bbh || !set_size(dirty)) return 0;
    if (bbh->visited != _visit_counter) bbh->visited = _visit_counter;
    else {
        _collect_local_out_function_reg_usage(dirty, save, bbh, off);
        return 0;
    }

    _collect_local_out_function_reg_usage(dirty, save, bbh, off);

    set_t copy;

    set_copy(&copy, dirty);
    _collect_out_function_reg_usage(&copy, save, bbh->l, NULL);
    set_free(&copy);
    
    set_copy(&copy, dirty);
    _collect_out_function_reg_usage(&copy, save, bbh->jmp, NULL);
    set_free(&copy);

    return 0;
}

static int _is_function_arg(lir_registers_t r) {
    r = LIR_format_register(r, CONF_get_full_bytness());
    lir_registers_t dec_abi_regs[]  = { RDI,  RSI,  RDX,  RCX,  R8,   R9 };
    lir_registers_t simd_abi_regs[] = { XMM0, XMM1, XMM2, XMM3, XMM4, XMM5, XMM6, XMM7 };
    for (int i = 0; i < (int)(sizeof(dec_abi_regs) / sizeof(dec_abi_regs[0])); i++) {
        if (dec_abi_regs[i] == r) return 1;
    }
    for (int i = 0; i < (int)(sizeof(simd_abi_regs) / sizeof(simd_abi_regs[0])); i++) {
        if (simd_abi_regs[i] == r) return 1;
    }
    return 0;
}

static inline int _same_full_register(lir_subject_t* a, lir_subject_t* b) {
    if (!a || !b || a->t != LIR_REGISTER || b->t != LIR_REGISTER) return 0;
    return LIR_format_register(a->storage.reg.reg, CONF_get_full_bytness()) == LIR_format_register(b->storage.reg.reg, CONF_get_full_bytness());
}

static inline lir_block_t* _find_pre_argload(lir_block_t* lh, lir_block_t* ex) {
    lir_subject_t* last = NULL;
    while (lh && lh != ex) {
        if (lh->op == LIR_PUSH) {
            last = lh->farg;
            goto _next_inst;
        }
        if (
            !last && LIR_is_movop(lh->op) && 
            lh->farg && lh->farg->t == LIR_REGISTER && 
            _is_function_arg(lh->farg->storage.reg.reg)
        ) goto _next_inst;
        if (last && LIR_is_movop(lh->op) && (LIR_subj_equals(last, lh->farg) || _same_full_register(last, lh->farg))) {
            last = NULL;
            goto _next_inst;
        }
        return lh;
_next_inst: {}
        lh = lh->prev;
    }

    return NULL;
}

static int _count_post_argload_pushes(lir_block_t* pre, lir_block_t* call) {
    int count = 0;
    lir_block_t* lh = pre ? pre->next : NULL;
    while (lh && lh != call) {
        lir_block_t* next = lh->next;
        if (
            lh->op == LIR_PUSH && lh->farg && 
            lh->farg->t == LIR_REGISTER && _is_function_arg(lh->farg->storage.reg.reg)
        ) count++;
        lh = next;
    }

    return count;
}

static inline lir_block_t* _find_post_argunload(lir_block_t* lh, lir_block_t* ex, int skip_pops) {
    while (lh && lh != ex) {
        if (lh->op == LIR_POP && skip_pops-- > 0) {
            lh = lh->next;
            continue;
        }

        if (lh->op != LIR_iADD) return lh;
        lh = lh->next;
    }

    return NULL;
}

int x86_64_macho_nasm_caller_saving(cfg_ctx_t* cctx, call_graph_t* calls, sym_table_t* smt) {
    foreach (cfg_func_t* fb, &cctx->funcs) {
        if (!fb->used) continue;
        foreach (cfg_block_t* bb, &fb->blocks) {
            iterate_lir_instructions (bb) {
                switch (lh->op) {
                    case LIR_FCLL:
                    case LIR_ECLL: {
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
                        set_t visited_funcs;
                        set_init(&visited_funcs, SET_NO_CMP);

                        while (queue_pop(&work_list, (void**)&func)) {
                            _collect_in_function_reg_usage(&func_regs, func, f_id, smt);
                            if (!func || set_has(&visited_funcs, (void*)func->f_id)) continue;
                            set_add(&visited_funcs, (void*)func->f_id);
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

                        long dirty_regs[32];
                        int dirty_count = 0;
                        if (bb->jmp) {
                            set_foreach (long reg, &func_regs) {
                                if (dirty_count >= (int)(sizeof(dirty_regs) / sizeof(dirty_regs[0]))) break;
                                dirty_regs[dirty_count++] = reg;
                            }
                        }

                        _collect_out_function_reg_usage(&func_regs, &save_regs, bb, lh->next);
                        for (int i = 0; i < dirty_count; i++) {
                            set_add(&save_regs, (void*)dirty_regs[i]);
                        }
                        
                        set_free(&visited_funcs);
                        queue_free(&work_list);
                        
                        lir_block_t* pre  = _find_pre_argload(lh->prev, bb->lmap.exit);
                        lir_block_t* post = _find_post_argunload(lh->next, bb->lmap.exit, _count_post_argload_pushes(pre, lh));
                        
                        long regs[32];
                        int regs_count = 0;
                        set_foreach (long reg, &save_regs) {
                            if (
                                reg == RAX || reg == RSP || reg == RBP || 
                                regs_count >= (int)(sizeof(regs) / sizeof(regs[0]))
                            ) continue;
                            regs[regs_count++] = reg;
                        }

                        lir_block_t* push_pos = pre;
                        for (int i = 0; i < regs_count; i++) {
                            lir_block_t* push = LIR_create_block(LIR_PUSH, LIR_SUBJ_REG(regs[i], CONF_get_full_bytness()), NULL, NULL);
                            LIR_insert_block_after(push, push_pos);
                            push_pos = push;
                        }

                        for (int i = regs_count - 1; i >= 0; i--) {
                            LIR_insert_block_before(LIR_create_block(LIR_POP, LIR_SUBJ_REG(regs[i], CONF_get_full_bytness()), NULL, NULL), post);
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
