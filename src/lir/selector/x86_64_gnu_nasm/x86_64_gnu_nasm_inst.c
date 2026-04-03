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
Get a mov operation for given input operands. Will return the base, if we 
won't change anything.
Note: Base by default is the 'LIR_iMOV' operation.
Note 2: If we've choosen a base operation, we will set the size
        of the second argument to the size of the first argument.
Params:
    - `a` - Destination operand.
    - `b` - Source operand.
    - `smt` - Symtable.
    - `base` - Base operation.

Returns a mov operation that is valid for given args.
*/
static lir_operation_t _get_proper_mov(lir_subject_t* a, lir_subject_t* b, sym_table_t* smt, lir_operation_t base) {
    int to_float   = _is_simd_type(a, smt);
    int from_float = _is_simd_type(b, smt);

    if (to_float) {
        if (from_float) {
            if (b->size == 4 && a->size == 8)      return LIR_CVTSS2SD;
            else if (b->size == 8 && a->size == 4) return LIR_CVTSD2SS;
            else return base;
        } 
        else {
            if (a->size <= 4) return (b->size == 4) ? LIR_CVTTSS2SI : LIR_CVTTSD2SI;
            else return (b->size == 4) ? LIR_CVTTSS2SI : LIR_CVTTSD2SI;
        }
    }
    else {
        int from_sign = _is_sign_type(b, smt);
        int from_num  = b->t == LIR_NUMBER;
        if (from_num) return base;
        
        if (from_float) {
            if (b->size == 4) return LIR_CVTTSS2SI;
            else return LIR_CVTTSD2SI;
        }
        else {
            if (a->size <= b->size) {
                b->size = a->size;
                return base;
            }
            else {
                if (b->size == 4 && a->size == 8) return from_sign ? LIR_MOVSXD : base;
                else return from_sign ? LIR_MOVSX : LIR_MOVZX;
            }
        }
    }

    return base;
}

/*
We need to be sure that all movs are proper. For example, we can't
preserve some instructions that aren't valid in our architecture such
as 'mov sil, r15' or 'mov r15, sil', etc. 
Params:
    - `bb` - Current base block.
    - `smt` - Symtable.

Returns 1 if an operation was secceed, otherwise it will returns 0.
*/
static int _validate_size_movs(cfg_block_t* bb, sym_table_t* smt) {
    lir_block_t* lh = LIR_get_next(bb->lmap.entry, bb->lmap.exit, 0);
    while (lh) {
        switch (lh->op) {
            case LIR_iMOV: case LIR_aMOV: case LIR_fMOV: {
                lh->op = _get_proper_mov(lh->farg, lh->sarg, smt, lh->op);
                break;
            }
            default: break;
        }

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
    list_t syscall_regs;
    list_init(&syscall_regs);

    foreach (cfg_func_t* fb, &cctx->funcs) {
        if (!fb->used) continue;
        foreach (cfg_block_t* bb, &fb->blocks) {
            lir_block_t* lh = LIR_get_next(bb->lmap.entry, bb->lmap.exit, 0);
            while (lh) {
                switch (lh->op) {
                    case LIR_STSARG: {
                        int sys_regs[] = { RAX, RDI, RSI, RDX, R10, R8, R9 };
                        if (lh->sarg->storage.cnst.value >= (long)(sizeof(sys_regs) / sizeof(RAX))) break;
                        if (sys_regs[lh->sarg->storage.cnst.value] != RAX) {
                            LIR_insert_block_before(
                                LIR_create_block(LIR_PUSH, LIR_SUBJ_REG(sys_regs[lh->sarg->storage.cnst.value], 8), NULL, NULL), 
                                lh
                            );
                            list_add(&syscall_regs, (void*)((long)sys_regs[lh->sarg->storage.cnst.value]));
                        }

                        lir_subject_t* nfarg = create_tmp(sys_regs[lh->sarg->storage.cnst.value], lh->farg, smt, 8);
                        LIR_unload_subject(lh->sarg);
                        lh->op   = LIR_aMOV;
                        lh->sarg = lh->farg;
                        lh->farg = nfarg;
                        break;
                    }
                    case LIR_SYSC: {
                        foreach (lir_registers_t rest, &syscall_regs) {
                            LIR_insert_block_after(LIR_create_block(LIR_POP, LIR_SUBJ_REG(rest, 8), NULL, NULL), lh);
                        }

                        break;
                    }
                    case LIR_STARGLD: {
                        lir_subject_t* src = LIR_SUBJ_OFF(RBP, (lh->sarg->storage.cnst.value + 1) * -8, lh->farg->size);
                        LIR_unload_subject(lh->sarg);
                        lh->op   = LIR_iMOV;
                        lh->sarg = src;
                        break;
                    }
                    case LIR_STFARG: {
                        abi_argument_t target;
                        if (!_get_abi_argument(lh->sarg->storage.cnst.value, lh->farg, &target, smt)) lh->op = LIR_PUSH;
                        else {
                            lir_subject_t* nfarg = create_tmp(target.reg, lh->farg, smt, -1);
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
                        ) nfarg = create_tmp(target.reg, lh->farg, smt, -1); // TODO: 8?
                        else nfarg = LIR_SUBJ_OFF(RBP, target.off, lh->farg->size);

                        LIR_unload_subject(lh->sarg);
                        lh->op   = LIR_iMOV;
                        lh->sarg = nfarg;
                        break;
                    }
                    case LIR_LOADFRET: {
                        lh->op   = LIR_iMOV;
                        lh->sarg = create_tmp(RAX, lh->farg, smt, -1);
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
                        lir_subject_t* a = create_tmp(RAX, lh->sarg, smt, -1);
                        _insert_instruction_before(bb, LIR_create_block(LIR_iMOV, a, lh->sarg, NULL), lh);

                        lir_subject_t* oldres = lh->farg;
                        lh->farg = lh->sarg = a;
                        _insert_instruction_after(bb, LIR_create_block(LIR_iMOV, oldres, a, NULL), lh);
                        break;
                    }
                    case LIR_FRET:
                    case LIR_EXITOP: {
                        if (!lh->farg) break;
                        lir_subject_t* a = create_tmp(lh->op == LIR_FRET ? RAX : RDX, lh->farg, smt, -1);
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
                        lir_subject_t* a = create_tmp(RAX, lh->sarg, smt, -1);
                        _insert_instruction_before(bb, LIR_create_block(LIR_iMOV, a, lh->sarg, NULL), lh);

                        lir_subject_t* oldres = lh->farg;
                        lh->farg = lh->sarg = a;

                        if (lh->op != LIR_iMOD) _insert_instruction_after(bb, LIR_create_block(LIR_iMOV, oldres, a, NULL), lh);
                        else {
                            lh->farg = LIR_SUBJ_REG(RDX, lh->farg->size);
                            _insert_instruction_after(bb, LIR_create_block(LIR_iMOV, oldres, lh->farg, NULL), lh);
                        }

                        break;
                    }
                    case LIR_CMP: {
                        if (lh->farg->t == LIR_VARIABLE) break;
                        lir_subject_t* a = create_tmp(RAX, lh->farg, smt, -1);
                        _insert_instruction_before(bb, LIR_create_block(LIR_iMOV, a, lh->farg, NULL), lh);
                        lh->farg = a;
                        break;
                    }
                    case LIR_iLWR:
                    case LIR_iLRE:
                    case LIR_iLRG:
                    case LIR_iLGE:
                    case LIR_iCMP:
                    case LIR_iNMP: {
                        lir_subject_t* a   = create_tmp(RAX, lh->sarg, smt, -1);
                        lir_subject_t* b   = lh->targ;
                        lir_subject_t* res = LIR_SUBJ_REG(AL, 1);
                        _insert_instruction_before(bb, LIR_create_block(LIR_iMOV, a, lh->sarg, NULL), lh);
                        _insert_instruction_after(bb, LIR_create_block(LIR_iMOV, lh->farg, res, NULL), lh);

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
                    case LIR_TF64: case LIR_TF32: 
                    case LIR_TI64: case LIR_TI32: case LIR_TI16: case LIR_TI8:
                    case LIR_TU64: case LIR_TU32: case LIR_TU16: case LIR_TU8: {
                        lh->op = _get_proper_mov(lh->farg, lh->sarg, smt, LIR_iMOV);
                        break;
                    }
                    default: break;
                }

                lh = LIR_get_next(lh, bb->lmap.exit, 1);
            }

            _validate_size_movs(bb, smt);
        }
    }

    list_free(&syscall_regs);
    return 1;
}
