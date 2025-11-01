#include <lir/lirgens/lirgens.h>

static int _pass_params(lir_ctx_t* ctx, list_t* args, sym_table_t* smt) {
    int argnum = 0;
    list_iter_t it;
    list_iter_hinit(args, &it);
    hir_subject_t* s;
    while ((s = list_iter_next(&it))) {
        LIR_BLOCK1(ctx, LIR_STFARG, x86_64_format_variable(s));
    }

    return 1;
}

int x86_64_generate_func(lir_ctx_t* ctx, hir_block_t* h, sym_table_t* smt) {
    switch (h->op) {
        case HIR_FDCL: LIR_BLOCK1(ctx, LIR_FDCL, LIR_SUBJ_FUNCNAME(h->farg));          break;
        case HIR_FRET: LIR_BLOCK1(ctx, LIR_FRET, x86_64_format_variable(h->farg));     break;
        case HIR_FARGLD: LIR_BLOCK1(ctx, LIR_LOADFARG, x86_64_format_variable(h->farg)); break;

        case HIR_FCLL:
        case HIR_ECLL: 
        case HIR_STORE_FCLL:
        case HIR_STORE_ECLL: {
            _pass_params(ctx, &h->targ->storage.list.h, smt);
            LIR_BLOCK1(ctx, LIR_FCLL, LIR_SUBJ_FUNCNAME(h->sarg));
            if (
                h->op == HIR_STORE_FCLL || h->op == HIR_STORE_ECLL
            ) LIR_BLOCK1(ctx, LIR_LOADFRET, x86_64_format_variable(h->farg));
            break;
        }

        case HIR_SYSC: 
        case HIR_STORE_SYSC: {
            _pass_params(ctx, &h->targ->storage.list.h, smt);
            LIR_BLOCK0(ctx, LIR_SYSC);
            if (
                h->op == HIR_STORE_SYSC
            ) LIR_BLOCK1(ctx, LIR_LOADFRET, x86_64_format_variable(h->farg));
            break;
        }

        default: break;
    }
}
