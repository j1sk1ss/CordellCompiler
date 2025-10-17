#include <lir/x86_64_gnu_nasm/opt/x86_64_movopt.h>

static int _check_rewrite(lir_block_t* curr, lir_subject_t* src) {
    lir_block_t* h = curr;
    while (h) {
        if (h->op == LIR_iMOV && LIR_subj_equals(src, h->farg)) return 1;
        if (LIR_subj_equals(src, h->farg) || LIR_subj_equals(src, h->sarg)) break;
        if (LIR_jmp_instruction(h->op)) break;
        if (src->t == LIR_REGISTER) {
            if (h->op == LIR_SYSC && LIR_sysc_reg(src->storage.reg.reg)) break;
            if (h->op == LIR_FCLL && LIR_funccall_reg(src->storage.reg.reg)) break;
        }

        h = h->next;
    }

    return 0;
}

int x86_64_clean_mov(lir_ctx_t* ctx) {
    lir_block_t* h = ctx->h;
    while (h) {
        if (h->op == LIR_iMOV && _check_rewrite(h->next, h->farg)) h->unused = 1;
        h = h->next;
    }

    return 1;   
}
