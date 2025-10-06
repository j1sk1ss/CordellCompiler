#include <asm/x86_64_gnu_nasm/x86_64_asmgen.h>

int x86_64_generate_data(sym_table_t* smt, FILE* output) {
    fprintf(output, "section .data\n");
    fprintf(output, "section .rodata\n");
    fprintf(output, "section .bss\n");
    fprintf(output, "section .text\n");
    return 1;
}
