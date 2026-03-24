#include <lir/selector/x84_64_gnu_nasm.h>
// TODO: Complete AVX support
/*
Checks a variable if it is a signed or not.
Params:
    - `s` - LIR subject.
    - `smt` - Symtable.

Return 1 either this is a signed variable or this isn't variable at all.
*/
static int _is_sign_type(lir_subject_t* s, sym_table_t* smt) {
    if (s->t != LIR_VARIABLE && s->t != LIR_GLVARIABLE) return 1;
    variable_info_t vi;
    if (!VRTB_get_info_id(s->storage.var.v_id, &vi, &smt->v)) return 1;
    switch (vi.type) {
        case U64_TYPE_TOKEN: case U32_TYPE_TOKEN:
        case U16_TYPE_TOKEN: case U8_TYPE_TOKEN: return 0;
        default: return 1;
    }
}

/*
Checks a variable is SIMD by the provided variable ID. 
Note: SIMD in this context is a set of variable types such as 
      F64 (tmp/stack/glb) and F32(tmp/stack/glb).
Params:
    - `vid` - Variable ID.
    - `smt` - Symtable.

Return 1 if the variable is SIMD.
*/
static int _is_simd_type(lir_subject_t* s, sym_table_t* smt) {
    if (s->t == LIR_NUMBER) return s->storage.num.is_float;
    if (s->t != LIR_VARIABLE && s->t != LIR_GLVARIABLE) return 0;
    variable_info_t vi;
    if (!VRTB_get_info_id(s->storage.var.v_id, &vi, &smt->v)) return 0;
    switch (vi.type) {
        case F64_TYPE_TOKEN: case F32_TYPE_TOKEN: return 1;
        default: return 0;
    }
}

/*
Create a temporary virtual variable that is linked to a physical register. 
Note: Virtual variable is a copy of the existed one from the LIR.
Params:
    - `reg` - Physical register.
    - `src` - Virtual variable base.
    - `smt` - Symtable.

Return the virtual variable that is linked to the physical register.
*/
static lir_subject_t* _create_tmp(lir_registers_t reg, lir_subject_t* src, sym_table_t* smt) {
    variable_info_t vi = { .vmi.offset = -1 };
    token_type_t vtype = TMP_TYPE_TOKEN;
    if (
        src->t == LIR_VARIABLE && 
        VRTB_get_info_id(src->storage.var.v_id, &vi, &smt->v)
    ) vtype = vi.type;
    symbol_id_t cpy = VRTB_add_info(NULL, vtype, NO_SYMBOL_ID, NULL, &smt->v);
    VRTB_update_memory(cpy, vi.vmi.offset, src->size, reg, FIELD_NO_CHANGE, &smt->v);
    return LIR_SUBJ_VAR(cpy, src->size);
}

/*
Insert block before 'pos' block with the block entry update.
Params:
    - `bb` - Source block.
    - `b` - New block for an insertion process.
    - `pos` - Position for an insert.
*/
static inline void _insert_instruction_before(cfg_block_t* bb, lir_block_t* b, lir_block_t* pos) {
    if (!b) return;
    if (bb->lmap.entry == pos) bb->lmap.entry = b;
    LIR_insert_block_before(b, pos);
}

/*
Insert block after 'pos' block with the block exit update.
Params:
    - `bb` - Source block.
    - `b` - New block for an insertion process.
    - `pos` - Position for an insert.
*/
static inline void _insert_instruction_after(cfg_block_t* bb, lir_block_t* b, lir_block_t* pos) {
    if (!b) return;
    if (bb->lmap.exit == pos) bb->lmap.exit = b;
    LIR_insert_block_after(b, pos);
}

/*
After the instruction selection we should be sure that this LIR is valid. 
Valid LIR implies that there is no wrong instructions such as movs "from mem to mem", 
ops "mem with mem", etc.
In a nutshell, this function doesn't do anything special. It just adds additional movs to 
temporary registers before critical operations.
Params:
    - `bb` - Current base block.
    - `smt` - Symtable.

Returns 1 if an operation was secceed, otherwise it will returns 0.
*/
static int _validate_selected_instuction(cfg_block_t* bb, sym_table_t* smt) {
    lir_block_t* lh = LIR_get_next(bb->lmap.entry, bb->lmap.exit, 0);
    while (lh) {
        lir_block_t* fix = NULL;
        switch (lh->op) {
            case LIR_REF: {
                lir_subject_t* tmp = _create_tmp(R15, lh->sarg, smt);
                fix = LIR_create_block(lh->op, tmp, lh->sarg, NULL);
                lh->sarg = tmp;
                lh->op   = LIR_iMOV;
                break;
            }
            case LIR_iMOV:  case LIR_aMOV: case LIR_fMOV: 
            case LIR_GDREF: case LIR_LDREF: {
                lir_subject_t* tmp = _create_tmp(R15, lh->sarg, smt);
                fix = LIR_create_block(LIR_iMOV, tmp, lh->sarg, NULL);
                lh->sarg = tmp;
                break;
            }
            default: break;
        }

        _insert_instruction_before(bb, fix, lh);
        lh = LIR_get_next(lh, bb->lmap.exit, 1);
    }

    return 1;
}

typedef struct {
    int reg;
    int off;
} abi_argument_t;

/*
Generate the information which will tell where we should put a value for a function.
Params:
    - `index` - Argument index.
    - `s` - Target value which will be placed to a function.
    - `out` - Output information placeholder.
    - `smt` - Symtable.

Returns 1 if this is a register value, otherwise (stack) will return 0.
*/
static int _get_abi_argument(int index, lir_subject_t* s, abi_argument_t* out, sym_table_t* smt) {
    int dec_abi_regs[]  = { RDI,  RSI,  RDX,  RCX,  R8,   R9 };
    int simd_abi_regs[] = { XMM0, XMM1, XMM2, XMM3, XMM4, XMM5, XMM6, XMM7 };

    int is_float = 0;
    switch (s->t) {
        case LIR_VARIABLE: is_float = _is_simd_type(s, smt);   break;
        case LIR_NUMBER:   is_float = s->storage.num.is_float; break;
        default: break;
    }

    if (!is_float) {
        if (index >= (long)(sizeof(dec_abi_regs) / sizeof(RDI))) {
            out->off = (index - (long)(sizeof(dec_abi_regs) / sizeof(RDI)) + 1) * -8;
            return 0;
        }
        else {
            out->reg = dec_abi_regs[index];
            return 1;
        }
    }
    else {
        if (index >= (long)(sizeof(simd_abi_regs) / sizeof(XMM0))) {
            out->off = (index - (long)(sizeof(simd_abi_regs) / sizeof(XMM0)) + 1) * -8;
            return 0;
        }
        else {
            out->reg = simd_abi_regs[index];
            return 1;
        }
    }

    return 0;
}

int x86_64_gnu_nasm_instruction_selection(cfg_ctx_t* cctx, sym_table_t* smt) {
    foreach (cfg_func_t* fb, &cctx->funcs) {
        if (!fb->used) continue;
        foreach (cfg_block_t* bb, &fb->blocks) {
            lir_block_t* lh = LIR_get_next(bb->lmap.entry, bb->lmap.exit, 0);
            while (lh) {
                switch (lh->op) {
                    case LIR_STSARG: {
                        int sys_regs[] = { RAX, RDI, RSI, RDX, R10, R8, R9 };
                        if (lh->sarg->storage.cnst.value >= (long)(sizeof(sys_regs) / sizeof(RAX))) break;
                        lir_subject_t* nfarg = _create_tmp(sys_regs[lh->sarg->storage.cnst.value], lh->farg, smt);
                        LIR_unload_subject(lh->sarg);
                        lh->op   = LIR_aMOV;
                        lh->sarg = lh->farg;
                        lh->farg = nfarg;
                        break;
                    }
                    case LIR_STARGLD: {
                        lir_subject_t* src = LIR_SUBJ_OFF(
                            RBP, (lh->sarg->storage.cnst.value + 1) * -8, _get_variable_size(lh->farg->storage.var.v_id, smt)
                        );

                        LIR_unload_subject(lh->sarg);
                        lh->op   = LIR_iMOV;
                        lh->sarg = src;
                        break;
                    }
                    case LIR_STFARG: {
                        abi_argument_t target;
                        if (!_get_abi_argument(lh->sarg->storage.cnst.value, lh->farg, &target, smt)) lh->op = LIR_PUSH;
                        else {
                            lir_subject_t* nfarg = _create_tmp(target.reg, lh->farg, smt);
                            LIR_unload_subject(lh->sarg);
                            lh->op   = LIR_aMOV;
                            lh->sarg = lh->farg;
                            lh->farg = nfarg;
                        }
                        
                        break;
                    }
                    case LIR_LOADFARG: {
                        abi_argument_t target;
                        lir_subject_t* nfarg;
                        if (
                            _get_abi_argument(lh->sarg->storage.cnst.value, lh->farg, &target, smt)
                        ) nfarg = _create_tmp(target.reg, lh->farg, smt);
                        else nfarg = LIR_SUBJ_OFF(RBP, target.off, _get_variable_size(lh->farg->storage.var.v_id, smt));

                        LIR_unload_subject(lh->sarg);
                        lh->op   = LIR_iMOV;
                        lh->sarg = nfarg;
                        break;
                    }
                    case LIR_LOADFRET: {
                        lh->op   = LIR_iMOV;
                        lh->sarg = _create_tmp(RAX, lh->farg, smt);
                        break;
                    }
                    case LIR_NOT:
                    case LIR_bOR:
                    case LIR_bXOR:
                    case LIR_bAND:
                    case LIR_iMUL:
                    case LIR_iSUB:
                    case LIR_iADD: {
                        /* Now the first argument (the result) in the next instruction, 
                           the third without any changes, and the second in the instruction
                           before. With this logic, we change the next instruction:
                           ```cpl
                           something = a <op> b;
                           ```
                           into the next sequence:
                           ```cpl
                           mov rax, a;
                           (rax = ) op rax, b;
                           mov something, rax;
                           ``` */
                        lir_subject_t* a = _create_tmp(RAX, lh->sarg, smt);
                        _insert_instruction_before(bb, LIR_create_block(LIR_iMOV, a, lh->sarg, NULL), lh);

                        lir_subject_t* oldres = lh->farg;
                        lh->farg = lh->sarg = a;
                        _insert_instruction_after(bb, LIR_create_block(LIR_iMOV, oldres, a, NULL), lh);
                        break;
                    }
                    case LIR_FRET:
                    case LIR_EXITOP: {
                        if (!lh->farg) break;
                        lir_subject_t* a = _create_tmp(lh->op == LIR_FRET ? RAX : RDX, lh->farg, smt);
                        _insert_instruction_before(bb, LIR_create_block(LIR_iMOV, a, lh->farg, NULL), lh);
                        lh->farg = a;
                        break;
                    }
                    case LIR_iDIV:
                    case LIR_iMOD: {
                        /* (rdx = ) xor rdx, rdx 
                           We need to clear the 'rdx' register before this operation.
                           See specifications of div operation on x86-64 GNU */
                        _insert_instruction_before(bb, LIR_create_block(LIR_bXOR, LIR_SUBJ_REG(RDX, 8), LIR_SUBJ_REG(RDX, 8), LIR_SUBJ_REG(RDX, 8)), lh);
                        lir_subject_t* a = _create_tmp(RAX, lh->sarg, smt);
                        _insert_instruction_before(bb, LIR_create_block(LIR_iMOV, a, lh->sarg, NULL), lh);

                        lir_subject_t* oldres = lh->farg;
                        lh->farg = lh->sarg = a;

                        if (lh->op != LIR_iMOD) _insert_instruction_after(bb, LIR_create_block(LIR_iMOV, oldres, a, NULL), lh);
                        else {
                            lh->farg = LIR_SUBJ_REG(RDX, _get_variable_size(lh->farg->storage.var.v_id, smt));
                            _insert_instruction_after(bb, LIR_create_block(LIR_iMOV, oldres, lh->farg, NULL), lh);
                        }

                        break;
                    }
                    case LIR_iLWR:
                    case LIR_iLRE:
                    case LIR_iLRG:
                    case LIR_iLGE:
                    case LIR_iCMP:
                    case LIR_iNMP: {
                        lir_subject_t* a   = _create_tmp(RAX, lh->sarg, smt);
                        lir_subject_t* b   = lh->targ;
                        lir_subject_t* res = LIR_SUBJ_REG(AL, 1);
                        _insert_instruction_before(bb, LIR_create_block(LIR_iMOV, a, lh->sarg, NULL), lh);
                        _insert_instruction_after(bb, LIR_create_block(LIR_MOVZX, lh->farg, res, NULL), lh);

                        switch (lh->op) {
                            case LIR_iCMP: _insert_instruction_after(bb, LIR_create_block(LIR_SETE, res, NULL, NULL), lh); break;
                            case LIR_iNMP: _insert_instruction_after(bb, LIR_create_block(LIR_STNE, res, NULL, NULL), lh); break;
                            default: {
                                if (_is_sign_type(lh->sarg, smt) && _is_sign_type(lh->targ, smt)) {
                                    switch (lh->op) {
                                        case LIR_iLWR: _insert_instruction_after(bb, LIR_create_block(LIR_SETL, res, NULL, NULL), lh); break;
                                        case LIR_iLRE: _insert_instruction_after(bb, LIR_create_block(LIR_STLE, res, NULL, NULL), lh); break;
                                        case LIR_iLRG: _insert_instruction_after(bb, LIR_create_block(LIR_SETG, res, NULL, NULL), lh); break;
                                        case LIR_iLGE: _insert_instruction_after(bb, LIR_create_block(LIR_STGE, res, NULL, NULL), lh); break;
                                        default: break;
                                    }
                                }
                                else {
                                    switch (lh->op) {
                                        case LIR_iLWR: _insert_instruction_after(bb, LIR_create_block(LIR_SETB, res, NULL, NULL), lh); break;
                                        case LIR_iLRE: _insert_instruction_after(bb, LIR_create_block(LIR_STBE, res, NULL, NULL), lh); break;
                                        case LIR_iLRG: _insert_instruction_after(bb, LIR_create_block(LIR_SETA, res, NULL, NULL), lh); break;
                                        case LIR_iLGE: _insert_instruction_after(bb, LIR_create_block(LIR_STAE, res, NULL, NULL), lh); break;
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
                        int from_float = _is_simd_type(lh->sarg, smt);
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
                        int from_float = _is_simd_type(lh->sarg, smt);
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

                lh = LIR_get_next(lh, bb->lmap.exit, 1);
            }

            _validate_selected_instuction(bb, smt);
        }
    }

    return 1;
}
