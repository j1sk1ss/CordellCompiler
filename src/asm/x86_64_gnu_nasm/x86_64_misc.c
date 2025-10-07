#include <asm/x86_64_gnu_nasm/x86_64_asmgen.h>

int x86_64_generate_stackframe(lir_block_t* h, lir_operation_t end, FILE* output) {
    long maxoff = 0;
    while (h) {
        if (h->farg && h->farg->t == LIR_MEMORY) maxoff = MAX(maxoff, h->farg->storage.var.offset);
        if (h->sarg && h->sarg->t == LIR_MEMORY) maxoff = MAX(maxoff, h->sarg->storage.var.offset);
        if (h->targ && h->targ->t == LIR_MEMORY) maxoff = MAX(maxoff, h->targ->storage.var.offset);
        if (h->op == end) break;
        h = h->next;
    }

    fprintf(output, "push rbp\n");
    fprintf(output, "mov rbp, rsp\n");
    fprintf(output, "sub rsp, %d\n", maxoff);
    return maxoff;
}

int x86_64_kill_stackframe(FILE* output) {
    fprintf(output, "mov rsp, rbp\n");
    fprintf(output, "pop rbp\n");
    return 1;
}
