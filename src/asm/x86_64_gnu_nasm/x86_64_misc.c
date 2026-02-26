#include <asm/x86_64_asmgen.h>

int x86_64_generate_stackframe(long offset, FILE* output) {
    fprintf(output, "push rbp\n");
    fprintf(output, "mov rbp, rsp\n");
    if (offset > 0) {
        fprintf(output, "sub rsp, %ld\n", ALIGN(offset, 8));
    }
    
    return offset;
}

int x86_64_kill_stackframe(FILE* output) {
    fprintf(output, "mov rsp, rbp\n");
    fprintf(output, "pop rbp\n");
    return 1;
}
