#include <asm/x86_64_gnu_nasm/x86_64_asmgen.h>

static int _allocate_data(int glob, int ro, int bss, sym_table_t* smt, FILE* output) {
    map_iter_t it;
    map_iter_init(&smt->v.vartb, &it);
    variable_info_t* vi;
    while (map_iter_next(&it, (void**)&vi)) {
        if (!vi->glob && glob || !vi->ro && ro) continue;

        /* Array and string allocation */
        if (vi->type == ARRAY_TYPE_TOKEN || vi->type == STR_TYPE_TOKEN) {
            array_info_t ai;
            if (!ARTB_get_info(vi->v_id, &ai, &smt->a)) continue;
            token_t tmptkn = { .t_type = vi->type, .flags = { .ptr = vi->ptr, .ro = vi->ro } };

            if (!list_size(&ai.elems) && !bss || list_size(&ai.elems) && bss) continue;
            if (!list_size(&ai.elems)) {
                switch (TKN_variable_bitness(&tmptkn, 1)) {
                    case 64: fprintf(output, "%s resq %d\n", vi->name, ai.size); break;
                    case 32: fprintf(output, "%s resd %d\n", vi->name, ai.size); break;
                    case 16: fprintf(output, "%s resw %d\n", vi->name, ai.size); break;
                    default: fprintf(output, "%s resb %d\n", vi->name, ai.size); break;
                }
            }
            else {
                switch (TKN_variable_bitness(&tmptkn, 1)) {
                    case 64: fprintf(output, "%s dq ", vi->name); break;
                    case 32: fprintf(output, "%s dd ", vi->name); break;
                    case 16: fprintf(output, "%s dw ", vi->name); break;
                    default: fprintf(output, "%s db ", vi->name); break;
                }

                int elcount = ai.size;
                list_iter_t elit;
                list_iter_hinit(&ai.elems, &elit);
                hir_subject_t* el;
                while ((el = list_iter_next(&elit))) {
                    fprintf(output, "%s", el->storage.num.value);
                    if (list_iter_current(&elit)) fprintf(output, ",");
                    elcount--;
                }

                if (elcount != ai.size && elcount) fprintf(output, ",");
                while (elcount-- > 0) {
                    fprintf(output, "0");
                    if (elcount) fprintf(output, ",");
                }

                fprintf(output, "\n");
            }

            continue;
        }

        /* Variable allocation */
        token_t tmptkn = { .t_type = vi->type, .flags = { .ptr = vi->ptr, .ro = vi->ro } };
        switch (TKN_variable_bitness(&tmptkn, 1)) {
            case 64: fprintf(output, "%s dq 0\n", vi->name); break;
            case 32: fprintf(output, "%s dd 0\n", vi->name); break;
            case 16: fprintf(output, "%s dw 0\n", vi->name); break;
            default: fprintf(output, "%s db 0\n", vi->name); break;
        }
    }

    return 1;
}

int x86_64_generate_data(sym_table_t* smt, FILE* output) {
    map_iter_t it;
    
    fprintf(output, "section .data\n");
    _allocate_data(1, 0, 0, smt, output);

    fprintf(output, "section .rodata\n");
    _allocate_data(0, 1, 0, smt, output);
    
    map_iter_init(&smt->s.strtb, &it);
    str_info_t* si;
    while (map_iter_next(&it, (void**)&si)) {
        if (si->t != STR_INDEPENDENT) continue;
        fprintf(output, "_str_%i_ db ", si->id);
        char* data = si->value;
        while (*data) {
            fprintf(output, "%i,", *data);
            data++;
        }

        fprintf(output, "0\n");
    }

    fprintf(output, "section .bss\n");
    _allocate_data(1, 0, 1, smt, output);

    fprintf(output, "section .text\n");

    map_iter_init(&smt->f.functb, &it);
    func_info_t* fi;
    while (map_iter_next(&it, (void**)&fi)) {
        if (fi->global)   fprintf(output, "global %s\n", fi->name);
        if (fi->external) fprintf(output, "extern %s\n", fi->name);
    }

    return 1;
}
