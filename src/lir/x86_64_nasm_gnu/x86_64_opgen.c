#include <lir/x86_64_gnu_nasm/x86_64_lirgen.h>

int _simd_binary_op(lir_ctx_t* ctx, hir_block_t* h, sym_table_t* smt) {
    LIR_reg_op(ctx, XMM0, RAX, LIR_iMOVq);
    LIR_reg_op(ctx, XMM1, RBX, LIR_iMOVq);

    switch (h->op) {
        case HIR_iSUB: {
            LIR_reg_op(ctx, XMM1, XMM0, LIR_fSUB);
            LIR_reg_op(ctx, XMM0, XMM1, LIR_fMVf);
            break;
        }
        
        case HIR_iDIV: {
            LIR_reg_op(ctx, XMM1, XMM0, LIR_fDIV);
            LIR_reg_op(ctx, XMM0, XMM1, LIR_fMVf);
            break;
        }

        case HIR_iMUL: LIR_reg_op(ctx, XMM0, XMM1, LIR_fMUL); break;
        case HIR_iADD: LIR_reg_op(ctx, XMM0, XMM1, LIR_fADD); break;
    }

    LIR_reg_op(ctx, RAX, XMM0, LIR_iMOVq);
    LIR_load_var_reg(LIR_iMOV, ctx, h->farg, RAX, smt);
    return 1;
}

int x86_64_generate_binary_op(lir_ctx_t* ctx, hir_block_t* h, sym_table_t* smt) {
    LIR_store_var_reg(LIR_iMOV, ctx, h->sarg, RAX, smt);
    LIR_store_var_reg(LIR_iMOV, ctx, h->targ, RBX, smt);
    if (HIR_is_floattype(h->farg->t)) return _simd_binary_op(ctx, h, smt);
    
    switch (h->op) {
        case HIR_iSUB: {
            LIR_reg_op(ctx, RBX, RAX, LIR_iSUB);
            LIR_reg_op(ctx, RAX, RBX, LIR_iMOV);
            break;
        }
        
        case HIR_iMOD:
        case HIR_iDIV: {
            LIR_BLOCK2(ctx, LIR_XCHG, LIR_SUBJ_REG(RAX, DEFAULT_TYPE_SIZE), LIR_SUBJ_REG(RBX, DEFAULT_TYPE_SIZE));
            if (HIR_is_signtype(h->sarg->t) && HIR_is_signtype(h->targ->t)) {
                LIR_BLOCK0(ctx, LIR_CDQ);
                LIR_BLOCK1(ctx, LIR_iDIV, LIR_SUBJ_REG(RBX, DEFAULT_TYPE_SIZE));
            } 
            else {
                LIR_BLOCK2(ctx, LIR_bXOR, LIR_SUBJ_REG(RDX, DEFAULT_TYPE_SIZE), LIR_SUBJ_REG(RDX, DEFAULT_TYPE_SIZE));
                LIR_BLOCK1(ctx, LIR_DIV, LIR_SUBJ_REG(RBX, DEFAULT_TYPE_SIZE));
            }

            if (h->op == HIR_iMOD) LIR_reg_op(ctx, RAX, RDX, LIR_iMOV);
            break;
        }

        case HIR_iMUL: LIR_reg_op(ctx, RAX, RBX, LIR_iMUL); break;
        case HIR_iADD: LIR_reg_op(ctx, RAX, RBX, LIR_iADD); break;
        case HIR_bOR:  LIR_reg_op(ctx, RAX, RBX, LIR_bOR);  break;
        case HIR_bXOR: LIR_reg_op(ctx, RAX, RBX, LIR_bXOR); break;
        case HIR_bAND: LIR_reg_op(ctx, RAX, RBX, LIR_bAND); break;

        case HIR_iLWR:
        case HIR_iLRE:
        case HIR_iLRG:
        case HIR_iLGE:
        case HIR_iCMP:
        case HIR_iNMP: {
            LIR_store_var_reg(LIR_iMOV, ctx, h->sarg, RAX, smt);
            LIR_store_var_reg(LIR_iMOV, ctx, h->targ, RBX, smt);
            if (HIR_is_floattype(h->farg->t)) LIR_reg_op(ctx, RAX, RBX, LIR_iCMP);
            else LIR_reg_op(ctx, XMM0, XMM1, LIR_fCMP);
            if (HIR_is_signtype(h->farg->t) && HIR_is_signtype(h->sarg->t)) {
                switch (h->op) {
                    case HIR_IFCPOP:  LIR_BLOCK1(ctx, LIR_SETE, LIR_SUBJ_REG(AL, 1)); break;
                    case HIR_IFNCPOP: LIR_BLOCK1(ctx, LIR_STNE, LIR_SUBJ_REG(AL, 1)); break;
                    case HIR_IFLWOP:  LIR_BLOCK1(ctx, LIR_SETL, LIR_SUBJ_REG(AL, 1)); break;
                    case HIR_IFLWEOP: LIR_BLOCK1(ctx, LIR_STLE, LIR_SUBJ_REG(AL, 1)); break;
                    case HIR_IFLGOP:  LIR_BLOCK1(ctx, LIR_SETG, LIR_SUBJ_REG(AL, 1)); break;
                    case HIR_IFLGEOP: LIR_BLOCK1(ctx, LIR_STGE, LIR_SUBJ_REG(AL, 1)); break;
                }
            }
            else {
                switch (h->op) {
                    case HIR_IFCPOP:  LIR_BLOCK1(ctx, LIR_SETE, LIR_SUBJ_REG(AL, 1)); break;
                    case HIR_IFNCPOP: LIR_BLOCK1(ctx, LIR_STNE, LIR_SUBJ_REG(AL, 1)); break;
                    case HIR_IFLWOP:  LIR_BLOCK1(ctx, LIR_SETB, LIR_SUBJ_REG(AL, 1)); break;
                    case HIR_IFLWEOP: LIR_BLOCK1(ctx, LIR_STBE, LIR_SUBJ_REG(AL, 1)); break;
                    case HIR_IFLGOP:  LIR_BLOCK1(ctx, LIR_SETA, LIR_SUBJ_REG(AL, 1)); break;
                    case HIR_IFLGEOP: LIR_BLOCK1(ctx, LIR_STAE, LIR_SUBJ_REG(AL, 1)); break;
                }
            }

            LIR_reg_op(ctx, RAX, AL, LIR_iMVZX);
            break;
        }
    }

    LIR_load_var_reg(LIR_iMOV, ctx, h->farg, RAX, smt);
    return 1;
}

int x86_64_generate_ifop(lir_ctx_t* ctx, hir_block_t* h, sym_table_t* smt) {
    switch (h->op) {
        case HIR_IFOP: {
            LIR_BLOCK2(ctx, HIR_iCMP, LIR_format_variable(h->farg, smt), LIR_SUBJ_CONST(0));
            LIR_BLOCK1(ctx, HIR_JMP, LIR_SUBJ_LABEL(h->sarg->id));
            break;
        }

        case HIR_IFCPOP:
        case HIR_IFNCPOP:
        case HIR_IFLWOP:
        case HIR_IFLWEOP:
        case HIR_IFLGOP:
        case HIR_IFLGEOP: {
            LIR_store_var_reg(LIR_iMOV, ctx, h->sarg, RAX, smt);
            LIR_store_var_reg(LIR_iMOV, ctx, h->targ, RBX, smt);
            if (HIR_is_floattype(h->farg->t)) LIR_reg_op(ctx, RAX, RBX, LIR_iCMP);
            else LIR_reg_op(ctx, XMM0, XMM1, LIR_fCMP);
            if (HIR_is_signtype(h->farg->t) && HIR_is_signtype(h->sarg->t)) {
                switch (h->op) {
                    case HIR_IFCPOP:  LIR_BLOCK1(ctx, LIR_JE,  LIR_SUBJ_LABEL(h->targ->id)); break;
                    case HIR_IFNCPOP: LIR_BLOCK1(ctx, LIR_JNE, LIR_SUBJ_LABEL(h->targ->id)); break;
                    case HIR_IFLWOP:  LIR_BLOCK1(ctx, LIR_JL,  LIR_SUBJ_LABEL(h->targ->id)); break;
                    case HIR_IFLWEOP: LIR_BLOCK1(ctx, LIR_JLE, LIR_SUBJ_LABEL(h->targ->id)); break;
                    case HIR_IFLGOP:  LIR_BLOCK1(ctx, LIR_JG,  LIR_SUBJ_LABEL(h->targ->id)); break;
                    case HIR_IFLGEOP: LIR_BLOCK1(ctx, LIR_JGE, LIR_SUBJ_LABEL(h->targ->id)); break;
                }
            }
            else {
                switch (h->op) {
                    case HIR_IFCPOP:  LIR_BLOCK1(ctx, LIR_JE,  LIR_SUBJ_LABEL(h->targ->id)); break;
                    case HIR_IFNCPOP: LIR_BLOCK1(ctx, LIR_JNE, LIR_SUBJ_LABEL(h->targ->id)); break;
                    case HIR_IFLWOP:  LIR_BLOCK1(ctx, LIR_JB,  LIR_SUBJ_LABEL(h->targ->id)); break;
                    case HIR_IFLWEOP: LIR_BLOCK1(ctx, LIR_JBE, LIR_SUBJ_LABEL(h->targ->id)); break;
                    case HIR_IFLGOP:  LIR_BLOCK1(ctx, LIR_JA,  LIR_SUBJ_LABEL(h->targ->id)); break;
                    case HIR_IFLGEOP: LIR_BLOCK1(ctx, LIR_JAE, LIR_SUBJ_LABEL(h->targ->id)); break;
                }
            }

            break;
        }

        default: break;
    }

    return 1;
}

int x86_64_generate_conv(lir_ctx_t* ctx, hir_block_t* h, sym_table_t* smt) {
    switch (h->op) {
        case HIR_TF64:
        case HIR_TF32: {
            if (!HIR_is_floattype(h->sarg->t)) {
                LIR_store_var_reg(LIR_iMOV, ctx, h->sarg, RAX, smt);
                LIR_reg_op(ctx, XMM0, RAX, LIR_TDBL);
                LIR_load_var_reg(LIR_iMOV, ctx, h->farg, XMM0, smt);
            }
            else {
                LIR_store_var_reg(LIR_iMOV, ctx, h->sarg, XMM0, smt);
                LIR_load_var_reg(LIR_iMOV, ctx, h->farg, XMM0, smt);
            }

            break;
        }

        case HIR_TI64:
        case HIR_TI32:
        case HIR_TI16:
        case HIR_TI8:
        case HIR_TU64:
        case HIR_TU32:
        case HIR_TU16:
        case HIR_TU8: {
            if (HIR_is_floattype(h->sarg->t)) {
                LIR_store_var_reg(LIR_iMOV, ctx, h->sarg, XMM0, smt);
                LIR_reg_op(ctx, RAX, XMM0, LIR_TINT);
                LIR_load_var_reg(LIR_iMOV, ctx, h->farg, RAX, smt);
            }
            else {
                LIR_store_var_reg(LIR_iMOV, ctx, h->sarg, RAX, smt);
                LIR_load_var_reg(LIR_iMOV, ctx, h->farg, RAX, smt);
            }

            break;
        }

        default: break;
    }

    return 1;
}
