#include <lir/selector/x84_64_gnu_nasm.h>
// TODO: Complete AVX support

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
        case LIR_VARIABLE: is_float = is_simd_type(s, smt);   break;
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
    queue_t dirty_regs;
    queue_init(&dirty_regs);

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
                            queue_push(&dirty_regs, (void*)((long)sys_regs[lh->sarg->storage.cnst.value]));
                        }

                        lir_subject_t* nfarg = create_tmp(sys_regs[lh->sarg->storage.cnst.value], lh->farg, smt, 8);
                        LIR_unload_subject(lh->sarg);
                        lh->op   = LIR_aMOV;
                        lh->sarg = lh->farg;
                        lh->farg = nfarg;
                        break;
                    }
                    case LIR_FCLL:
                    case LIR_SYSC: {
                        if (lh->op == LIR_SYSC) { /* https://stackoverflow.com/questions/50571275/why-does-a-syscall-clobber-rcx-and-r11 */
                            LIR_insert_block_before(LIR_create_block(LIR_PUSH, LIR_SUBJ_REG(RCX, 8), NULL, NULL), lh);
                            queue_push(&dirty_regs, (void*)((long)RCX));
                            LIR_insert_block_before(LIR_create_block(LIR_PUSH, LIR_SUBJ_REG(R11, 8), NULL, NULL), lh);
                            queue_push(&dirty_regs, (void*)((long)R11));
                        }

                        long dirty;
                        while (queue_pop(&dirty_regs, (void**)&dirty)) {
                            LIR_insert_block_after(LIR_create_block(LIR_POP, LIR_SUBJ_REG(dirty, 8), NULL, NULL), lh);
                        }

                        break;
                    }
                    case LIR_STARGLD: {
                        lir_subject_t* src;
                        switch (lh->sarg->storage.cnst.value) {
                            case 0: {
                                src = create_tmp(RDI, lh->farg, smt, -1);
                                lh->op = LIR_iMOV; 
                                break;
                            }
                            default: {
                                src = create_tmp(RSI, lh->farg, smt, -1);
                                lh->op = LIR_REF_GDREF;  
                                break;
                            }
                        }

                        LIR_unload_subject(lh->sarg);
                        lh->sarg = src;
                        break;
                    }
                    case LIR_STFARG: {
                        abi_argument_t target;
                        if (!_get_abi_argument(lh->sarg->storage.cnst.value, lh->farg, &target, smt)) lh->op = LIR_PUSH;
                        else {
                            lir_subject_t* nfarg = create_tmp(target.reg, lh->farg, smt, -1);
                            LIR_insert_block_before(LIR_create_block(LIR_PUSH, LIR_SUBJ_REG(target.reg, 8), NULL, NULL), lh);
                            queue_push(&dirty_regs, (void*)((long)target.reg));
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
                        ) nfarg = create_tmp(target.reg, lh->farg, smt, -1);
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
                    case LIR_NOT: {
                        lir_subject_t* a   = create_tmp(RAX, lh->sarg, smt, -1);
                        lir_subject_t* res = LIR_SUBJ_REG(AL, 1);
                        _insert_instruction_before(bb, LIR_create_block(LIR_iMOV, a, lh->sarg, NULL), lh);
                        _insert_instruction_before(bb, LIR_create_block(LIR_TST, a, a, NULL), lh);
                        _insert_instruction_before(bb, LIR_create_block(LIR_SETE, res, NULL, NULL), lh);
                        lh->op = LIR_iMOV;
                        lh->sarg = res;
                        break;
                    }
                    case LIR_iBRHT:
                    case LIR_iBLFT:
                    case LIR_iMUL:
                    case LIR_bOR:
                    case LIR_bXOR:
                    case LIR_bAND:
                    case LIR_iSUB:
                    case LIR_iADD: {
                        int shared_size = -1;
                        if (lh->op == LIR_iMUL) shared_size = lh->sarg->size < 4 ? 4 : lh->sarg->size; 
                        lir_subject_t* a_entry = create_tmp(RAX, lh->sarg, smt, shared_size);
                        lir_subject_t* a_exit  = create_tmp(RAX, lh->farg, smt, shared_size);
                        _insert_instruction_before(bb, LIR_create_block(LIR_iMOV, a_entry, lh->sarg, NULL), lh);

                        if (
                            lh->op == LIR_iBRHT || 
                            lh->op == LIR_iBLFT
                        ) {
                            lir_subject_t* b_entry = create_tmp(RCX, lh->targ, smt, 1);
                            _insert_instruction_before(bb, LIR_create_block(LIR_iMOV, b_entry, lh->targ, NULL), lh);
                            lh->targ = b_entry;
                        }

                        _insert_instruction_after(bb, LIR_create_block(LIR_iMOV, lh->farg, a_exit, NULL), lh);
                        lir_subject_t* a_middle = create_tmp(RAX, lh->sarg, smt, shared_size);
                        lh->farg = lh->sarg = a_middle;
                        break;
                    }
                    case LIR_FRET:
                    case LIR_EXITOP: {
                        if (!lh->farg) break;
                        lir_subject_t* a = create_tmp(lh->op == LIR_FRET ? RAX : RDI, lh->farg, smt, -1);
                        _insert_instruction_before(bb, LIR_create_block(LIR_iMOV, a, lh->farg, NULL), lh);
                        lh->farg = a;
                        break;
                    }
                    case LIR_iDIV:
                    case LIR_iMOD: {
                        lir_subject_t* a_entry = create_tmp(RAX, lh->sarg, smt, 8);
                        _insert_instruction_before(bb, LIR_create_block(LIR_iMOV, a_entry, lh->sarg, NULL), lh);

                        lir_subject_t* a_middle = create_tmp(RAX, lh->sarg, smt, 8);
                        lir_subject_t* oldres = lh->farg;
                        lh->farg = lh->sarg = a_middle;

                        lir_subject_t* b = create_tmp(RCX, lh->targ, smt, 8);
                        _insert_instruction_before(bb, LIR_create_block(LIR_iMOV, b, lh->targ, NULL), lh);
                        lh->targ = b;

                        lir_subject_t* mod = create_tmp(RDX, lh->farg, smt, 8);
                        _insert_instruction_before(bb, LIR_create_block(LIR_PUSH, mod, NULL, NULL), lh);
                        _insert_instruction_before(bb, LIR_create_block(LIR_CQO, NULL, NULL, NULL), lh);
                        if (lh->op != LIR_iMOD) {
                            _insert_instruction_after(bb, LIR_create_block(LIR_iMOV, oldres, LIR_SUBJ_REG(RAX, oldres->size), NULL), lh);
                            _insert_instruction_after(bb, LIR_create_block(LIR_POP, mod, NULL, NULL), lh);
                        }
                        else {
                            lh->farg = LIR_SUBJ_REG(RDX, lh->farg->size);
                            _insert_instruction_after(bb, LIR_create_block(LIR_iMOV, oldres, LIR_SUBJ_REG(RAX, oldres->size), NULL), lh);
                            _insert_instruction_after(bb, LIR_create_block(LIR_POP, mod, NULL, NULL), lh);
                            _insert_instruction_after(bb, LIR_create_block(LIR_iMOV, LIR_SUBJ_REG(RAX, lh->farg->size), lh->farg, NULL), lh);
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
                                if (is_sign_type(lh->sarg, smt) && is_sign_type(lh->targ, smt)) {
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
                        lh->op = get_proper_mov(lh->farg, lh->sarg, smt, LIR_iMOV);
                        break;
                    }
                    default: break;
                }

                lh = LIR_get_next(lh, bb->lmap.exit, 1);
            }
        }
    }

    queue_free(&dirty_regs);
    return 1;
}
