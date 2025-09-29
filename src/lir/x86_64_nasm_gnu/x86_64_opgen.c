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
    }

    LIR_load_var_reg(LIR_iMOV, ctx, h->farg, RAX, smt);
    return 1;
}
