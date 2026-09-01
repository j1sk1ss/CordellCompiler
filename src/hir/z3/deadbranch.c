#include <hir/z3opt.h>

static int _hide_branch(cfg_block_t* bb) {
    if (!bb) return 0;
    for (
        cfg_block_t* child = bb->dom_c; 
        child; child = child->dom_s
    ) _hide_branch(child);

    iterate_hir_instructions (bb) {
        if (
            hh->op == HIR_MKLB || 
            hh->op == HIR_FEND || 
            hh->op == HIR_STEND
        ) continue;
        hh->unused = 1;
    }

    return 1;
}

int Z3OPT_deadbranch(cfg_ctx_t* cctx, sym_table_t* smt) {
    z3_analyzer_t* z3 = Z3A_create(cctx, smt);
    if (!z3) return 1;

    foreach (cfg_func_t* fb, &cctx->funcs) {
        if (!fb->used) continue;
        foreach (cfg_block_t* bb, &fb->blocks) {
            iterate_hir_instructions (bb) {
                if (hh->op != HIR_IFOP2) continue;
#define SWAP_ARGS(src) do {          \
    hir_subject_t* __tmp = hh->farg; \
    hh->farg = src;                  \
    src      = __tmp;                \
} while (0);
                if (bb->l && (Z3_is_block_reachable(z3, fb, bb->l) == Z3A_NO)) {
                    _hide_branch(bb->l);
                    bb->l  = NULL;
                    hh->op = HIR_JMP;
                    SWAP_ARGS(hh->targ);
                }

                if (bb->jmp && (Z3_is_block_reachable(z3, fb, bb->jmp) == Z3A_NO)) {
                    _hide_branch(bb->jmp);
                    bb->jmp = bb->l;
                    bb->l   = NULL;
                    hh->op  = HIR_JMP;
                    SWAP_ARGS(hh->sarg);
                }
#undef SWAP_ARGS
            }
        }
    }

    Z3A_unload(z3);
    return 1;
}
