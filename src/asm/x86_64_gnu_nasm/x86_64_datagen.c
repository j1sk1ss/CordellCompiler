#include <asm/x86_64_gnu_nasm/x86_64_asmgen.h>

int x86_64_generate_data(sym_table_t* smt, FILE* output) {
    map_iter_t it;
    
    fprintf(output, "section .data\n");

    map_iter_init(&smt->v.vartb, &it);
    variable_info_t* vi;
    while (map_iter_next(&it, (void**)&vi)) {
        if (!vi->glob || vi->type == ARRAY_TYPE_TOKEN || vi->type == STR_TYPE_TOKEN) continue;
        token_t tmptkn = { .t_type = vi->type, .flags = { .ptr = vi->ptr, .ro = vi->ro, .glob = vi->glob } };
        switch (VRS_variable_bitness(&tmptkn, 1)) {
            case 64: fprintf(output, "%s dq 0\n", vi->name); break;
            case 32: fprintf(output, "%s dd 0\n", vi->name); break;
            case 16: fprintf(output, "%s dw 0\n", vi->name); break;
            default: fprintf(output, "%s db 0\n", vi->name); break;
        }
    }

    fprintf(output, "section .rodata\n");
    
    map_iter_init(&smt->s.strtb, &it);
    str_info_t* si;
    while (map_iter_next(&it, (void**)&si)) {
        if (si->t == STR_RAW_ASM) continue;
        fprintf(output, "_str_%i_ db ", si->id);
        char* data = si->value;
        while (*data) {
            fprintf(output, "%i,", *data);
            data++;
        }

        fprintf(output, "0\n");
    }

    fprintf(output, "section .bss\n");
    fprintf(output, "section .text\n");

    map_iter_init(&smt->f.functb, &it);
    func_info_t* fi;
    while (map_iter_next(&it, (void**)&fi)) {
        if (fi->global)   fprintf(output, "global %s\n", fi->name);
        if (fi->external) fprintf(output, "extern %s\n", fi->name);
    }

    return 1;
}
