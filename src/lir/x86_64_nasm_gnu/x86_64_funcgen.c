#include <lir/x86_64_gnu_nasm/x86_64_lirgen.h>

static int _load_registers(lir_ctx_t* ctx, int* abi_regs, sstack_t* params, int argc, sym_table_t* smt) {
    print_debug("_load_registers(arc=%i)", argc);
    for (int i = 0; i < argc; i++) {
        stack_elem_t se;
        stack_top_addr(params, &se);
        LIR_store_var_reg(LIR_iMOV, ctx, (hir_subject_t*)se.data.addrdata, abi_regs[i], smt);
        stack_pop(params);
    }

    return 1;
}

static const int _abi_regs[] = { RDI, RSI, RDX, RCX, R8, R9 };
static const int _sys_regs[] = { RAX, RDI, RSI, RDX, R10, R8, R9 };

int x86_64_generate_func(lir_ctx_t* ctx, hir_block_t* h, sym_table_t* smt, sstack_t* params) {
    switch (h->op) {
        case HIR_FDCL: {
            func_info_t fi;
            if (FNTB_get_info_id(h->farg->storage.str.s_id, &fi, &smt->f)) {
                if (fi.global) LIR_BLOCK1(ctx, LIR_MKGLB, LIR_SUBJ_FUNCNAME(h->farg));
                LIR_BLOCK1(ctx, LIR_FDCL, LIR_SUBJ_FUNCNAME(h->farg));
            }

            break;
        }

        case HIR_FRET: LIR_BLOCK1(ctx, LIR_FRET, LIR_format_variable(ctx, h->farg, smt));                       break;
        case HIR_FARGLD: LIR_load_var_reg(LIR_iMOV, ctx, h->farg, _abi_regs[h->sarg->storage.cnst.value], smt); break;

        case HIR_FCLL:
        case HIR_ECLL: 
        case HIR_STORE_FCLL:
        case HIR_STORE_ECLL: {
            for (int i = 0; i < FREE_REGISTERS; i++) {
                LIR_BLOCK1(ctx, LIR_PUSH, LIR_SUBJ_REG(R11 + i, DEFAULT_TYPE_SIZE));
            }

            int argc = h->sarg->storage.cnst.value;
            if (h->op == HIR_STORE_FCLL || h->op == HIR_STORE_ECLL) argc = h->targ->storage.cnst.value;
            _load_registers(ctx, (int*)_abi_regs, params, argc, smt);

            LIR_BLOCK1(ctx, LIR_FCLL, LIR_SUBJ_FUNCNAME(h->farg));
            if (h->op == HIR_STORE_FCLL || h->op == HIR_STORE_ECLL) {
                LIR_load_var_reg(LIR_iMOV, ctx, h->farg, RAX, smt);
            }

            for (int i = FREE_REGISTERS - 1; i >= 0; i--) {
                LIR_BLOCK1(ctx, LIR_POP, LIR_SUBJ_REG(R11 + i, DEFAULT_TYPE_SIZE));
            }
            
            break;
        }

        case HIR_SYSC: 
        case HIR_STORE_SYSC: {
            int argc = h->sarg->storage.cnst.value;
            if (h->op == HIR_STORE_SYSC) argc = h->targ->storage.cnst.value;
            _load_registers(ctx, (int*)_sys_regs, params, argc, smt);
            LIR_BLOCK0(ctx, LIR_SYSC);
            if (h->op == HIR_STORE_SYSC) {
                LIR_load_var_reg(LIR_iMOV, ctx, h->farg, RAX, smt);
            }

            break;
        }

        default: break;
    }
}
