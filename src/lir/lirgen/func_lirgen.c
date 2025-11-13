#include <lir/lirgens/lirgens.h>

static int _pass_params(lir_operation_t op, lir_ctx_t* ctx, list_t* args) {
    int argnum = 0;
    list_iter_t it;
    list_iter_hinit(args, &it);
    hir_subject_t* s;
    while ((s = list_iter_next(&it))) {
        LIR_BLOCK2(ctx, op, x86_64_format_variable(s), LIR_SUBJ_CONST(argnum++));
    }

    return 1;
}

int x86_64_generate_func(lir_ctx_t* ctx, hir_block_t* h) {
    switch (h->op) {
        case HIR_FDCL:   LIR_BLOCK1(ctx, LIR_FDCL, LIR_SUBJ_FUNCNAME(h->farg));                                                       break;
        case HIR_FRET:   LIR_BLOCK1(ctx, LIR_FRET, x86_64_format_variable(h->farg));                                                  break;
        case HIR_FARGLD: LIR_BLOCK2(ctx, LIR_LOADFARG, x86_64_format_variable(h->farg), LIR_SUBJ_CONST(h->sarg->storage.cnst.value)); break;

        case HIR_FCLL:
        case HIR_ECLL: 
        case HIR_STORE_FCLL:
        case HIR_STORE_ECLL: {
            _pass_params(LIR_STFARG, ctx, &h->targ->storage.list.h);
            LIR_BLOCK1(ctx, LIR_FCLL, LIR_SUBJ_FUNCNAME(h->sarg));
            if (h->op == HIR_STORE_FCLL || h->op == HIR_STORE_ECLL) {
                LIR_BLOCK1(ctx, LIR_LOADFRET, x86_64_format_variable(h->farg));
            }

            break;
        }

        case HIR_SYSC: 
        case HIR_STORE_SYSC: {
            _pass_params(LIR_STSARG, ctx, &h->targ->storage.list.h);
            LIR_BLOCK0(ctx, LIR_SYSC);
            if (h->op == HIR_STORE_SYSC) {
                LIR_BLOCK1(ctx, LIR_LOADFRET, x86_64_format_variable(h->farg));
            }
            
            break;
        }

        default: break;
    }

    return 1;
}
