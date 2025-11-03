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

static int _get_variable_size(long vid, sym_table_t* smt) {
    variable_info_t vi;
    if (VRTB_get_info_id(vid, &vi, &smt->v)) {
        switch (vi.type) {
            case I64_TYPE_TOKEN:
            case U64_TYPE_TOKEN:
            case F64_TYPE_TOKEN: return DEFAULT_TYPE_SIZE;
            case I32_TYPE_TOKEN:
            case U32_TYPE_TOKEN:
            case F32_TYPE_TOKEN: return 4;
            case I16_TYPE_TOKEN:
            case U16_TYPE_TOKEN: return 2;
            case I8_TYPE_TOKEN:
            case U8_TYPE_TOKEN:  return 1;
            default: break;
        }
    }

    return DEFAULT_TYPE_SIZE;
}

static lir_subject_t* _create_reg(int reg, int sz) {
    return LIR_SUBJ_REG(reg, sz);
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
                    case LIR_STARGLD: {
                        lir_subject_t* src = NULL;
                        switch (lh->sarg->storage.cnst.value) {
                            case 0: src = LIR_SUBJ_OFF(-8, _get_variable_size(lh->farg->storage.var.v_id, smt));  break;
                            case 1: src = LIR_SUBJ_OFF(-16, _get_variable_size(lh->farg->storage.var.v_id, smt)); break;
                        }

                        lh->op = LIR_iMOV;
                        LIR_unload_subject(lh->sarg);
                        lh->sarg = src;
                        break;
                    }

                    case LIR_STSARG: {
                        lh->op = LIR_iMOV;
                        lir_subject_t* src = _create_reg(_sys_regs[lh->sarg->storage.cnst.value], _get_variable_size(lh->farg->storage.var.v_id, smt));
                        LIR_unload_subject(lh->sarg);
                        lh->sarg = lh->farg;
                        lh->farg = src;
                        break;
                    }

                    case LIR_STFARG: {
                        lh->op = LIR_iMOV;
                        lir_subject_t* src = _create_reg(_abi_regs[lh->sarg->storage.cnst.value], _get_variable_size(lh->farg->storage.var.v_id, smt));
                        LIR_unload_subject(lh->sarg);
                        lh->sarg = lh->farg;
                        lh->farg = src;
                        break;
                    }

                    case LIR_LOADFARG: {
                        lh->op = LIR_iMOV;
                        lir_subject_t* src = _create_reg(_abi_regs[lh->sarg->storage.cnst.value], _get_variable_size(lh->farg->storage.var.v_id, smt));
                        LIR_unload_subject(lh->sarg);
                        lh->sarg = src;
                        break;
                    }

                    case LIR_LOADFRET: {
                        lh->op = LIR_iMOV;
                        lir_subject_t* src = _create_reg(RAX, _get_variable_size(lh->farg->storage.var.v_id, smt));
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
                            a = _create_reg(RAX, _get_variable_size(lh->sarg->storage.var.v_id, smt));
                            b = _create_reg(RBX, _get_variable_size(lh->targ->storage.var.v_id, smt));
                            LIR_insert_block_before(LIR_create_block(LIR_iMOV, a, lh->sarg, NULL), lh);
                            LIR_insert_block_before(LIR_create_block(LIR_iMOV, b, lh->targ, NULL), lh);
                        }
                        else {
                            a = _create_reg(XMM0, _get_variable_size(lh->sarg->storage.var.v_id, smt));
                            b = _create_reg(XMM1, _get_variable_size(lh->targ->storage.var.v_id, smt));
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

                    case LIR_iDIV:
                    case LIR_iMOD: {
                        lir_subject_t *a, *b, *mod;
                        if (!_is_simd_type(lh->farg->storage.var.v_id, smt)) {
                            a = _create_reg(RAX, _get_variable_size(lh->sarg->storage.var.v_id, smt));
                            b = _create_reg(RBX, _get_variable_size(lh->targ->storage.var.v_id, smt));
                            LIR_insert_block_before(LIR_create_block(LIR_iMOV, a, lh->sarg, NULL), lh);
                            LIR_insert_block_before(LIR_create_block(LIR_iMOV, b, lh->targ, NULL), lh);
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
                            a   = _create_reg(RAX, _get_variable_size(lh->sarg->storage.var.v_id, smt));
                            b   = _create_reg(RBX, _get_variable_size(lh->targ->storage.var.v_id, smt));
                            res = _create_reg(AL, 1);
                            LIR_insert_block_before(LIR_create_block(LIR_iMOV, a, lh->sarg, NULL), lh);
                            LIR_insert_block_before(LIR_create_block(LIR_iMOV, b, lh->targ, NULL), lh);
                        }

                        LIR_insert_block_after(LIR_create_block(LIR_iMVZX, lh->farg, res, NULL), lh);

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
                                    }
                                }
                                else {
                                    switch (lh->op) {
                                        case LIR_iLWR: LIR_insert_block_after(LIR_create_block(LIR_SETB, res, NULL, NULL), lh); break;
                                        case LIR_iLRE: LIR_insert_block_after(LIR_create_block(LIR_STBE, res, NULL, NULL), lh); break;
                                        case LIR_iLRG: LIR_insert_block_after(LIR_create_block(LIR_SETA, res, NULL, NULL), lh); break;
                                        case LIR_iLGE: LIR_insert_block_after(LIR_create_block(LIR_STAE, res, NULL, NULL), lh); break;
                                    }
                                }
                            }
                        }

                        lh->op = LIR_CMP;
                        lh->farg = a;
                        lh->sarg = b;
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

int x86_64_gnu_nasm_register_selection(cfg_ctx_t* cctx, map_t* colors, sym_table_t* smt) {
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
                if (lh->op == LIR_VRDEALL) {

                }

                lir_subject_t* args[] = { lh->farg, lh->sarg, lh->targ };
                for (int i = 0; i < 3; i++) {
                    if (!args[i]) continue;
                    if (args[i]->t != LIR_VARIABLE) continue;

                    variable_info_t vi;
                    if (!VRTB_get_info_id(args[i]->storage.var.v_id, &vi, &smt->v)) continue;
                    if (vi.glob) {
                        args[i]->t = LIR_GLVARIABLE;
                        continue;
                    }

                    long color;
                    vi.vmi.size = _get_variable_size(vi.v_id, smt);
                    if (map_get(colors, args[i]->storage.var.v_id, (void**)&color)) {
                        if (color < _registers_count) {
                            args[i]->t = LIR_REGISTER;
                            args[i]->storage.reg.reg = _registers[color];
                            vi.vmi.reg = args[i]->storage.reg.reg;
                        }
                        else {
                            args[i]->t = LIR_MEMORY;
                            args[i]->storage.var.offset = !vi.vmi.allocated ? stack_map_alloc(vi.vmi.size, &smp) : vi.vmi.offset;
                            vi.vmi.offset = args[i]->storage.var.offset;
                        }

                        if (vi.vmi.allocated) continue;
                        VRTB_update_memory(vi.v_id, vi.vmi.offset, vi.vmi.size, vi.vmi.reg, &smt->v);
                    }
                }

                if (lh == bb->lmap.exit) break;
                lh = lh->next;
            }
        }
    }

    return 1;
}
