#include <asm/x86_64_gnu_nasm/x86_64_asmgen.h>

int x86_64_generate_data(sym_table_t* smt, FILE* output) {
    map_iter_t it;
    
    fprintf(output, "section .data\n");
    fprintf(output, "section .rodata\n");
    fprintf(output, "section .bss\n");
    fprintf(output, "section .text\n");

    map_iter_init(&smt->f.functb, &it);
    func_info_t* fi;
    while (map_iter_next(&it, (void**)&fi)) {
        if (fi->global)   fprintf(output, "global _cpl_%s\n", fi->name);
        if (fi->external) fprintf(output, "extern %s\n", fi->name);
    }

    return 1;
}
