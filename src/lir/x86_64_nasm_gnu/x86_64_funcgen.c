#include <lir/x86_64_gnu_nasm/x86_64_lirgen.h>

static int _load_registers(lir_ctx_t* ctx, int* abi_regs, list_t* args, sym_table_t* smt) {
    int argnum = 0;
    list_iter_t it;
    list_iter_hinit(args, &it);
    hir_subject_t* s;
    while ((s = list_iter_next(&it))) {
        x86_64_store_var_reg(LIR_iMOV, ctx, s, abi_regs[argnum++], -1, smt);
    }

    return 1;
}

static const int _abi_regs[] = { RDI, RSI, RDX, RCX, R8, R9 };
static const int _sys_regs[] = { RAX, RDI, RSI, RDX, R10, R8, R9 };

int x86_64_generate_func(lir_ctx_t* ctx, hir_block_t* h, sym_table_t* smt) {
    switch (h->op) {
        case HIR_FDCL: {
            func_info_t fi;
            if (FNTB_get_info_id(h->farg->storage.str.s_id, &fi, &smt->f)) {
                LIR_BLOCK1(ctx, LIR_FDCL, LIR_SUBJ_FUNCNAME(h->farg));
            }

            break;
        }

        case HIR_FRET: {
            if (h->farg) x86_64_store_var_reg(LIR_iMOV, ctx, h->farg, RAX, -1, smt);
            LIR_BLOCK0(ctx, LIR_FRET);
            break;
        }

        case HIR_FARGLD: x86_64_load_var_reg(LIR_iMOV, ctx, h->farg, _abi_regs[h->sarg->storage.cnst.value], -1, smt); break;

        case HIR_FCLL:
        case HIR_ECLL: 
        case HIR_STORE_FCLL:
        case HIR_STORE_ECLL: {
            _load_registers(ctx, (int*)_abi_regs, &h->targ->storage.list.h, smt);
            for (int i = 0; i < FREE_REGISTERS; i++) LIR_BLOCK1(ctx, LIR_PUSH, LIR_SUBJ_REG(FIRST_FREE_REGISTER + i, DEFAULT_TYPE_SIZE));
            LIR_BLOCK1(ctx, LIR_FCLL, LIR_SUBJ_FUNCNAME(h->sarg));
            for (int i = FREE_REGISTERS - 1; i >= 0; i--) LIR_BLOCK1(ctx, LIR_POP, LIR_SUBJ_REG(FIRST_FREE_REGISTER + i, DEFAULT_TYPE_SIZE));
            if (h->op == HIR_STORE_FCLL || h->op == HIR_STORE_ECLL) x86_64_load_var_reg(LIR_iMOV, ctx, h->farg, RAX, -1, smt);
            break;
        }

        case HIR_SYSC: 
        case HIR_STORE_SYSC: {
            _load_registers(ctx, (int*)_sys_regs, &h->targ->storage.list.h, smt);
            x86_64_store_var_reg(LIR_iMOV, ctx, h->targ->storage.list.h.h->data, RAX, -1, smt);
            LIR_BLOCK0(ctx, LIR_SYSC);
            if (h->op == HIR_STORE_SYSC) x86_64_load_var_reg(LIR_iMOV, ctx, h->farg, RAX, -1, smt);
            break;
        }

        default: break;
    }
}
