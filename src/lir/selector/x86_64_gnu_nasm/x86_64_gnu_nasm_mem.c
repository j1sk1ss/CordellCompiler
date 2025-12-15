#include <lir/selector/x84_64_gnu_nasm.h>

static int _update_subject_memory(lir_subject_t* s, stack_map_t* smp, map_t* colors, sym_table_t* smt) {
    variable_info_t vi;
    if (!VRTB_get_info_id(s->storage.var.v_id, &vi, &smt->v)) return 0;
    if (vi.glob) {
        s->t = LIR_GLVARIABLE;
        return 1;
    }
    
    long color;
    vi.vmi.size = _get_variable_size(vi.v_id, smt);
    if (!vi.vmi.allocated && map_get(colors, s->storage.var.v_id, (void**)&color)) {
        vi.vmi.offset = stack_map_alloc(vi.vmi.size, smp);
        vi.vmi.reg    = -1;
        VRTB_update_memory(vi.v_id, vi.vmi.offset, vi.vmi.size, vi.vmi.reg, &smt->v);
    }

    if (vi.vmi.offset >= 0) {
        s->t = LIR_MEMORY;
        s->storage.var.offset = vi.vmi.offset;
    }
    else if (vi.vmi.reg >= 0) {
        s->t = LIR_REGISTER;
        s->storage.reg.reg = vi.vmi.reg;
    }

    return 1;
}

int x86_64_gnu_nasm_memory_selection(cfg_ctx_t* cctx, map_t* colors, sym_table_t* smt) {
    stack_map_t smp;
    stack_map_init(0, &smp);

    foreach (cfg_func_t* fb, &cctx->funcs) {
        if (!fb->used) continue;
        foreach (cfg_block_t* bb, &fb->blocks) {
            lir_block_t* lh = LIR_get_next(bb->lmap.entry, bb->lmap.exit, 0);
            while (lh) {
                switch (lh->op) {
                    case LIR_VRDEALL: {
                        variable_info_t vi;
                        if (
                            !VRTB_get_info_id(lh->farg->storage.cnst.value, &vi, &smt->v) || 
                            vi.glob || vi.vmi.offset == -1
                        ) {
                            lh->unused = 1;
                            break;
                        }

                        stack_map_free(vi.vmi.offset, vi.vmi.size, &smp);
                        break;
                    }
                    case LIR_STRDECL: {
                        variable_info_t vi;
                        if (!VRTB_get_info_id(lh->farg->storage.var.v_id, &vi, &smt->v)) break;
                        if (vi.glob) break;

                        str_info_t si;
                        array_info_t ai;
                        if (
                            STTB_get_info_id(lh->sarg->storage.str.sid, &si, &smt->s) &&
                            ARTB_get_info(lh->farg->storage.var.v_id, &ai, &smt->a)
                        ) {
                            int arroff = stack_map_alloc(ai.size, &smp);
                            VRTB_update_memory(lh->farg->storage.var.v_id, arroff, ai.size, vi.vmi.reg, &smt->v);
                            char* string = si.value->body;
                            while (*string) {
                                LIR_insert_block_before(
                                    LIR_create_block(LIR_iMOV, LIR_SUBJ_OFF(arroff--, 1), LIR_SUBJ_CONST(*(string++)), NULL), lh
                                );
                            }

                            LIR_insert_block_before(LIR_create_block(LIR_iMOV, LIR_SUBJ_OFF(arroff, 1), LIR_SUBJ_CONST(0), NULL), lh);
                        }

                        lh->unused = 1;
                        break;
                    }
                    case LIR_ARRDECL: {
                        variable_info_t vi;
                        if (!VRTB_get_info_id(lh->farg->storage.var.v_id, &vi, &smt->v)) break;
                        if (vi.glob) break;

                        array_info_t ai;
                        if (ARTB_get_info(lh->farg->storage.var.v_id, &ai, &smt->a)) {
                            int elsize = _get_ast_type_size(ai.el_type);
                            int arroff = stack_map_alloc(ai.size * elsize, &smp);
                            VRTB_update_memory(lh->farg->storage.var.v_id, arroff, ai.size, vi.vmi.reg, &smt->v);

                            int pos = 0;
                            foreach (lir_subject_t* elem, &lh->targ->storage.list.h) {
                                if (elem->t == LIR_VARIABLE) _update_subject_memory(elem, &smp, colors, smt);
                                LIR_insert_block_before(
                                    LIR_create_block(LIR_iMOV, LIR_SUBJ_OFF(arroff - pos * elsize, 1), elem, NULL), lh
                                );

                                pos++;
                            }
                        }

                        lh->unused = 1;
                        break;
                    }
                    default: {
                        lir_subject_t* args[] = { lh->farg, lh->sarg, lh->targ };
                        for (int i = 0; i < 3; i++) {
                            if (!args[i] || args[i]->t != LIR_VARIABLE) continue;
                            _update_subject_memory(args[i], &smp, colors, smt);
                        }

                        break;
                    }
                }

                lh = LIR_get_next(lh, bb->lmap.exit, 1);
            }
        }
    }

    return 1;
}
