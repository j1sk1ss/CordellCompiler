#include <asm/x86_64_gnu_nasm/x86_64_asm_generator.h>

int x86_64_generate_asm(ir_block_t* h, FILE* output) {
    ir_block_t* curr = h;
    while (curr) {
        switch (curr->op) {
            case STRT: {
                iprintf(output, "global _start\n");
                iprintf(output, "_start:\n");
                break;
            }
            case SYSC: iprintf(output, "syscall\n"); break;
            default: break;
        }

        curr = curr->next;
    }

    return 1;
}
