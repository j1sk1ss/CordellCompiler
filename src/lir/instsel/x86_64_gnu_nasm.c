#include <lir/instsel/x84_64_gnu_nasm.h>

static const int _abi_regs[] = { RDI, RSI, RDX, RCX, R8, R9      };
static const int _sys_regs[] = { RAX, RDI, RSI, RDX, R10, R8, R9 };

static int _is_sign_type(lir_subject_t* s, sym_table_t* smt) {
    if (s->t != LIR_VARIABLE || s->t != LIR_GLVARIABLE) return 1;
    variable_info_t vi;
    if (VRTB_get_info_id(s->storage.var.v_id, &vi, &smt->v)) {
        switch (vi.type) {
            case U64_TYPE_TOKEN:
            case U32_TYPE_TOKEN:
            case U16_TYPE_TOKEN:
            case U8_TYPE_TOKEN: return 0;
            default: break;
        }
    }

    return 1;
}

static int _is_simd_type(long vid, sym_table_t* smt) {
    variable_info_t vi;
    if (VRTB_get_info_id(vid, &vi, &smt->v)) {
        switch (vi.type) {
            case F64_TYPE_TOKEN: 
            case F32_TYPE_TOKEN: return 1;
            default: break;
        }
    }

    return 0;
}

static int _get_ast_type_size(token_type_t t) {
    switch (t) {
        case TMP_I64_TYPE_TOKEN: case TMP_U64_TYPE_TOKEN: case TMP_F64_TYPE_TOKEN:
        case I64_TYPE_TOKEN:     case U64_TYPE_TOKEN:     case F64_TYPE_TOKEN: return DEFAULT_TYPE_SIZE;
        case TMP_I32_TYPE_TOKEN: case TMP_U32_TYPE_TOKEN: case TMP_F32_TYPE_TOKEN:
        case I32_TYPE_TOKEN:     case U32_TYPE_TOKEN:     case F32_TYPE_TOKEN: return 4;
        case TMP_I16_TYPE_TOKEN: case TMP_U16_TYPE_TOKEN:
        case I16_TYPE_TOKEN:     case U16_TYPE_TOKEN: return 2;
        case TMP_I8_TYPE_TOKEN:  case TMP_U8_TYPE_TOKEN:
        case I8_TYPE_TOKEN:      case U8_TYPE_TOKEN: return 1;
        default: return DEFAULT_TYPE_SIZE;
    }
}

static int _get_variable_size(long vid, sym_table_t* smt) {
    variable_info_t vi;
    if (VRTB_get_info_id(vid, &vi, &smt->v)) {
        if (vi.ptr) return DEFAULT_TYPE_SIZE;
        return _get_ast_type_size(vi.type);
    }

    return DEFAULT_TYPE_SIZE;
}

static inline lir_subject_t* _create_reg(int reg, int sz) {
    return LIR_SUBJ_REG(reg, sz);
}

static inline lir_subject_t* _create_tmp(int reg, lir_subject_t* src, sym_table_t* smt) {
    long cpy;
    variable_info_t vi;
    if (
        src->t == LIR_VARIABLE && 
        VRTB_get_info_id(src->storage.var.v_id, &vi, &smt->v)
    ) cpy = VRTB_add_copy(&vi, &smt->v);
    else cpy = VRTB_add_info(NULL, TMP_TYPE_TOKEN, 0, NULL, &smt->v);

    VRTB_update_memory(cpy, vi.vmi.offset, src->size, reg, &smt->v);
    return LIR_SUBJ_VAR(cpy, src->size);
}

static inline lir_subject_t* _create_mem(int off, int sz) {
    return LIR_SUBJ_OFF(off, sz);
}

int x86_64_gnu_nasm_instruction_selection(cfg_ctx_t* cctx, sym_table_t* smt) {
    list_iter_t fit;
    list_iter_hinit(&cctx->funcs, &fit);
    cfg_func_t* fb;
    while ((fb = (cfg_func_t*)list_iter_next(&fit))) {
        if (!fb->used) continue;
        list_iter_t bit;
        list_iter_hinit(&fb->blocks, &bit);
        cfg_block_t* bb;
        while ((bb = (cfg_block_t*)list_iter_next(&bit))) {
            lir_block_t* lh = bb->lmap.entry;
            while (lh) {
                switch (lh->op) {
                    case LIR_STSARG:
                    case LIR_STFARG: {
                        int* src_regs = (int*)_abi_regs;
                        if (lh->op == LIR_STSARG) src_regs = (int*)_sys_regs;
                        lir_subject_t* src = _create_tmp(
                            src_regs[lh->sarg->storage.cnst.value], 
                            lh->farg, smt
                        );

                        lh->op = LIR_iMOV;
                        LIR_unload_subject(lh->sarg);
                        lh->sarg = lh->farg;
                        lh->farg = src;
                        break;
                    }

                    case LIR_STARGLD:
                    case LIR_LOADFARG: {
                        lir_subject_t* src = NULL;
                        if (lh->op == LIR_STARGLD) src = LIR_SUBJ_OFF(
                            (lh->sarg->storage.cnst.value + 1) * -8, 
                            _get_variable_size(lh->farg->storage.var.v_id, smt)
                        );
                        else src = _create_tmp(
                            _abi_regs[lh->sarg->storage.cnst.value], 
                            lh->farg, smt
                        );

                        lh->op = LIR_iMOV;
                        LIR_unload_subject(lh->sarg);
                        lh->sarg = src;
                        break;
                    }

                    case LIR_LOADFRET: {
                        lir_subject_t* src = _create_tmp(RAX, lh->farg, smt);
                        lh->op = LIR_iMOV;
                        lh->sarg = src;
                        break;
                    }

                    case LIR_bOR:
                    case LIR_bXOR:
                    case LIR_bAND:
                    case LIR_iMUL:
                    case LIR_iSUB:
                    case LIR_iADD: {
                        lir_subject_t *a, *b;
                        if (!_is_simd_type(lh->farg->storage.var.v_id, smt)) {
                            a = _create_tmp(RAX, lh->sarg, smt);
                            b = lh->targ;
                            LIR_insert_block_before(LIR_create_block(LIR_iMOV, a, lh->sarg, NULL), lh);
                        }
                        else {
                            a = _create_tmp(XMM0, lh->sarg, smt);
                            b = _create_tmp(XMM1, lh->targ, smt);
                            switch (lh->op) {
                                case LIR_iMUL: lh->op = LIR_fMUL; break;
                                case LIR_iSUB: lh->op = LIR_fSUB; break;
                                case LIR_iADD: lh->op = LIR_fADD; break;
                                default: break;
                            }
                        }

                        lir_subject_t* oldres = lh->farg;

                        lh->farg = a;
                        lh->sarg = a;
                        lh->targ = b;

                        LIR_insert_block_after(LIR_create_block(LIR_iMOV, oldres, a, NULL), lh);
                        break;
                    }

                    case LIR_EXITOP:
                    case LIR_FRET: {
                        lir_subject_t* a = _create_tmp(lh->op == LIR_FRET ? RAX : RDX, lh->farg, smt);
                        LIR_insert_block_before(LIR_create_block(LIR_iMOV, a, lh->farg, NULL), lh);
                        lh->farg = a;
                        break;
                    }

                    case LIR_iDIV:
                    case LIR_iMOD: {
                        lir_subject_t *a, *b, *mod;
                        LIR_insert_block_before(LIR_create_block(LIR_bXOR, _create_reg(RDX, 8), _create_reg(RDX, 8), NULL), lh);
                        if (!_is_simd_type(lh->farg->storage.var.v_id, smt)) {
                            a = _create_tmp(RAX, lh->sarg, smt);
                            b = lh->targ;
                            LIR_insert_block_before(LIR_create_block(LIR_iMOV, a, lh->sarg, NULL), lh);
                        }
                        else {
                            a = _create_reg(XMM0, _get_variable_size(lh->sarg->storage.var.v_id, smt));
                            b = _create_reg(XMM1, _get_variable_size(lh->targ->storage.var.v_id, smt));
                            switch (lh->op) {
                                case LIR_iDIV: lh->op = LIR_fDIV; break;
                                default: break;
                            }
                        }

                        if (lh->op == LIR_iMOD) {
                            mod = _create_reg(RDX, _get_variable_size(lh->farg->storage.var.v_id, smt));
                            LIR_insert_block_before(LIR_create_block(LIR_bXOR, mod, mod, mod), lh);
                        }

                        lir_subject_t* oldres = lh->farg;
                        
                        if (lh->op == LIR_iMOD) lh->farg = mod;
                        else lh->farg = a;

                        lh->sarg = a;
                        lh->targ = b;

                        if (lh->op == LIR_iMOD) LIR_insert_block_after(LIR_create_block(LIR_iMOV, oldres, mod, NULL), lh);
                        else LIR_insert_block_after(LIR_create_block(LIR_iMOV, oldres, a, NULL), lh);
                        break;
                    }

                    case LIR_iLWR:
                    case LIR_iLRE:
                    case LIR_iLRG:
                    case LIR_iLGE:
                    case LIR_iCMP:
                    case LIR_iNMP: {
                        lir_subject_t *a, *b, *res;
                        if (!_is_simd_type(lh->farg->storage.var.v_id, smt)) {
                            a   = _create_tmp(RAX, lh->sarg, smt);
                            b   = lh->targ;
                            res = _create_reg(AL, 1);
                            LIR_insert_block_before(LIR_create_block(LIR_iMOV, a, lh->sarg, NULL), lh);
                        }

                        LIR_insert_block_after(LIR_create_block(LIR_MOVZX, lh->farg, res, NULL), lh);

                        switch (lh->op) {
                            case LIR_iCMP: LIR_insert_block_after(LIR_create_block(LIR_SETE, res, NULL, NULL), lh); break;
                            case LIR_iNMP: LIR_insert_block_after(LIR_create_block(LIR_STNE, res, NULL, NULL), lh); break;
                            default: {
                                if (_is_sign_type(lh->sarg, smt) && _is_sign_type(lh->targ, smt)) {
                                    switch (lh->op) {
                                        case LIR_iLWR: LIR_insert_block_after(LIR_create_block(LIR_SETL, res, NULL, NULL), lh); break;
                                        case LIR_iLRE: LIR_insert_block_after(LIR_create_block(LIR_STLE, res, NULL, NULL), lh); break;
                                        case LIR_iLRG: LIR_insert_block_after(LIR_create_block(LIR_SETG, res, NULL, NULL), lh); break;
                                        case LIR_iLGE: LIR_insert_block_after(LIR_create_block(LIR_STGE, res, NULL, NULL), lh); break;
                                        default: break;
                                    }
                                }
                                else {
                                    switch (lh->op) {
                                        case LIR_iLWR: LIR_insert_block_after(LIR_create_block(LIR_SETB, res, NULL, NULL), lh); break;
                                        case LIR_iLRE: LIR_insert_block_after(LIR_create_block(LIR_STBE, res, NULL, NULL), lh); break;
                                        case LIR_iLRG: LIR_insert_block_after(LIR_create_block(LIR_SETA, res, NULL, NULL), lh); break;
                                        case LIR_iLGE: LIR_insert_block_after(LIR_create_block(LIR_STAE, res, NULL, NULL), lh); break;
                                        default: break;
                                    }
                                }
                            }
                        }

                        lh->op = LIR_CMP;
                        lh->farg = a;
                        lh->sarg = b;
                        break;
                    }

                    case LIR_TF64:
                    case LIR_TF32: {
                        int from_float = _is_simd_type(lh->sarg->storage.var.v_id, smt);
                        int dst_size   = _get_variable_size(lh->farg->storage.var.v_id, smt);
                        int src_size   = _get_variable_size(lh->sarg->storage.var.v_id, smt);
                        if (from_float) {
                            if (src_size == 4 && dst_size == 8)      lh->op = LIR_CVTSS2SD;
                            else if (src_size == 8 && dst_size == 4) lh->op = LIR_CVTSD2SS;
                            else lh->op = LIR_iMOV;
                        } 
                        else {
                            if (dst_size <= 4) lh->op = (src_size == 4) ? LIR_CVTTSS2SI : LIR_CVTTSD2SI;
                            else lh->op = (src_size == 4) ? LIR_CVTTSS2SI : LIR_CVTTSD2SI;
                        }

                        break;
                    }

                    case LIR_TI64:
                    case LIR_TI32:
                    case LIR_TI16:
                    case LIR_TI8:
                    case LIR_TU64:
                    case LIR_TU32:
                    case LIR_TU16:
                    case LIR_TU8: {
                        int from_float = _is_simd_type(lh->sarg->storage.var.v_id, smt);
                        int from_sign  = _is_sign_type(lh->sarg, smt);
                        int dst_size   = _get_variable_size(lh->farg->storage.var.v_id, smt);
                        int src_size   = _get_variable_size(lh->sarg->storage.var.v_id, smt);
                        if (from_float) {
                            if (src_size == 4) lh->op = LIR_CVTTSS2SI;
                            else lh->op = LIR_CVTTSD2SI;
                        }
                        else {
                            if (dst_size <= src_size) lh->op = LIR_iMOV;
                            else {
                                if (src_size == 4 && dst_size == 8) lh->op = LIR_MOVSXD;
                                else lh->op = from_sign ? LIR_MOVSX : LIR_MOVZX;
                            }
                        }

                        break;
                    }

                    default: break;
                }

                if (lh == bb->lmap.exit) break;
                lh = lh->next;
            }
        }
    }

    return 1;
}

static const int _registers[] = { RAX, RBX, RCX, RDX, RSI, RDI, R8, R9, R10, R11, R12, R13, R14, R15 };
static const int _registers_count = 14;

static int _update_subject_memory(lir_subject_t* s, stack_map_t* smp, map_t* colors, sym_table_t* smt) {
    variable_info_t vi;
    if (!VRTB_get_info_id(s->storage.var.v_id, &vi, &smt->v)) return 0;
    if (vi.glob) {
        s->t = LIR_GLVARIABLE;
        return 1;
    }
    
    long color;
    vi.vmi.size = _get_variable_size(vi.v_id, smt);
    if (map_get(colors, s->storage.var.v_id, (void**)&color)) {
        if (color < _registers_count && color >= 0) {
            s->t = LIR_REGISTER;
            s->storage.reg.reg = _registers[color];
            vi.vmi.reg = s->storage.reg.reg;
        }
        else {
            s->t = LIR_MEMORY;
            s->storage.var.offset = !vi.vmi.allocated ? stack_map_alloc(vi.vmi.size, smp) : vi.vmi.offset;
            vi.vmi.offset = s->storage.var.offset;
        }

        if (vi.vmi.allocated) return 1;
        VRTB_update_memory(vi.v_id, vi.vmi.offset, vi.vmi.size, vi.vmi.reg, &smt->v);
    }

    return 1;
}

int x86_64_gnu_nasm_memory_selection(cfg_ctx_t* cctx, map_t* colors, sym_table_t* smt) {
    stack_map_t smp;
    stack_map_init(0, &smp);

    list_iter_t fit;
    list_iter_hinit(&cctx->funcs, &fit);
    cfg_func_t* fb;
    while ((fb = (cfg_func_t*)list_iter_next(&fit))) {
        if (!fb->used) continue;
        list_iter_t bit;
        list_iter_hinit(&fb->blocks, &bit);
        cfg_block_t* bb;
        while ((bb = (cfg_block_t*)list_iter_next(&bit))) {
            lir_block_t* lh = bb->lmap.entry;
            while (lh) {
                switch (lh->op) {
                    case LIR_VRDEALL: {
                        variable_info_t vi;
                        if (!VRTB_get_info_id(lh->farg->storage.cnst.value, &vi, &smt->v) || vi.glob || vi.vmi.offset == -1) {
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
                            char* string = si.value;
                            while (*string) {
                                LIR_insert_block_before(
                                    LIR_create_block(LIR_iMOV, _create_mem(arroff--, 1), LIR_SUBJ_CONST(*(string++)), NULL), lh
                                );
                            }

                            LIR_insert_block_before(LIR_create_block(LIR_iMOV, _create_mem(arroff, 1), LIR_SUBJ_CONST(0), NULL), lh);
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
                            list_iter_t elem_it;
                            list_iter_hinit(&lh->targ->storage.list.h, &elem_it);
                            lir_subject_t* elem;
                            while ((elem = list_iter_next(&elem_it))) {
                                if (elem->t == LIR_VARIABLE) _update_subject_memory(elem, &smp, colors, smt);
                                LIR_insert_block_before(
                                    LIR_create_block(LIR_iMOV, _create_mem(arroff - pos * elsize, 1), elem, NULL), lh
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

                if (lh == bb->lmap.exit) break;
                lh = lh->next;
            }
        }
    }

    return 1;
}
