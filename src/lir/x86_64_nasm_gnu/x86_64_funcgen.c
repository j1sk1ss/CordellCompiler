#include <lir/x86_64_gnu_nasm/x86_64_lirgen.h>

int x86_64_generate_func(lir_ctx_t* ctx, hir_block_t* h, sym_table_t* smt, sstack_t* params) {
    switch (h->op) {
        case HIR_FDCL: {
            func_info_t fi;
            if (FNTB_get_info_id(h->farg->storage.str.s_id, &fi, &smt->f)) {
                if (fi.global) LIR_BLOCK1(ctx, LIR_MKGLB, LIR_SUBJ_STRING(h->farg->storage.str.s_id));
                LIR_BLOCK1(ctx, LIR_FDCL, LIR_SUBJ_FUNCNAME(h->farg));
            }

            break;
        }

        case HIR_FRET: {
            int vrsize = LIR_get_hirtype_size(h->farg->t);
            LIR_store_var_reg(LIR_iMOV, ctx, h->farg, RAX, smt);
            LIR_BLOCK0(ctx, LIR_FRET);
            break;
        }

        case HIR_FARGLD: {
            static const int abi_regs[] = { RDI, RSI, RDX, RCX, R8, R9 };
            LIR_load_var_reg(LIR_iMOV, ctx, h->farg, abi_regs[h->sarg->storage.cnst.value], smt);
            break;
        }

        case HIR_FCLL:
        case HIR_ECLL: {
            static const int abi_regs[] = { R9, R8, RCX, RDX, RSI, RDI };
            for (int i = 0; i < h->sarg->storage.cnst.value; i++) {
                stack_elem_t se;
                stack_top_addr(params, &se);
                if (!se.data.addrdata) break;
                LIR_store_var_reg(LIR_iMOV, ctx, (hir_subject_t*)se.data.addrdata, abi_regs[i], smt);
                stack_pop(params);
            }

            LIR_BLOCK1(ctx, LIR_FCLL, LIR_SUBJ_FUNCNAME(h->farg));
            break;
        }

        case HIR_STORE_FCLL:
        case HIR_STORE_ECLL: {
            static const int abi_regs[] = { R9, R8, RCX, RDX, RSI, RDI };
            for (int i = 0; i < h->targ->storage.cnst.value; i++) {
                stack_elem_t se;
                stack_top_addr(params, &se);
                if (!se.data.addrdata) break;
                LIR_store_var_reg(LIR_iMOV, ctx, (hir_subject_t*)se.data.addrdata, abi_regs[i], smt);
                stack_pop(params);
            }

            LIR_BLOCK1(ctx, LIR_FCLL, LIR_SUBJ_FUNCNAME(h->sarg));
            LIR_load_var_reg(LIR_iMOV, ctx, h->farg, RAX, smt);
            break;
        }

        default: break;
    }
}