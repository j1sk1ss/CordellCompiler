#include <asm/x86_64_asmgen.h>

static int _allocate_data(int glob, int ro, int bss, sym_table_t* smt, FILE* output) {
    map_foreach (variable_info_t* vi, &smt->v.vartb) {
        if ((!vi->vfs.glob && glob) || (!vi->vfs.ro && ro)) continue;
        if (vi->type == ARRAY_TYPE_TOKEN || vi->type == STR_TYPE_TOKEN) {
            array_info_t ai;
            if (!ARTB_get_info(vi->v_id, &ai, &smt->a)) continue;
            token_t tmptkn = { .t_type = ai.elements_info.el_type, .flags = { .ptr = ai.elements_info.el_flags.ptr } };
            if ((!list_size(&ai.elems) && !bss) || (list_size(&ai.elems) && bss)) continue;
            if (!list_size(&ai.elems)) {
                switch (TKN_variable_bitness(&tmptkn, 1)) {
                    case 64: fprintf(output, "%s resq %ld\n", vi->name->body, ai.size); break;
                    case 32: fprintf(output, "%s resd %ld\n", vi->name->body, ai.size); break;
                    case 16: fprintf(output, "%s resw %ld\n", vi->name->body, ai.size); break;
                    default: fprintf(output, "%s resb %ld\n", vi->name->body, ai.size); break;
                }
            }
            else {
                switch (TKN_variable_bitness(&tmptkn, 1)) {
                    case 64: fprintf(output, "%s dq ", vi->name->body); break;
                    case 32: fprintf(output, "%s dd ", vi->name->body); break;
                    case 16: fprintf(output, "%s dw ", vi->name->body); break;
                    default: fprintf(output, "%s db ", vi->name->body); break;
                }

                int elcount = ai.size;
                foreach (array_elem_info_t* el, &ai.elems) {
                    fprintf(output, "%lu", el->value);
                    if (elcount--) fprintf(output, ",");
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

        token_t tmptkn = { .t_type = vi->type, .flags = { .ptr = vi->vfs.ptr, .ro = vi->vfs.ro } };
        switch (TKN_variable_bitness(&tmptkn, 1)) {
            case 64: fprintf(output, "%s dq 0\n", vi->name->body); break;
            case 32: fprintf(output, "%s dd 0\n", vi->name->body); break;
            case 16: fprintf(output, "%s dw 0\n", vi->name->body); break;
            default: fprintf(output, "%s db 0\n", vi->name->body); break;
        }
    }

    return 1;
}

int x86_64_generate_data(sym_table_t* smt, FILE* output) {
    fprintf(output, "section .data\n");
    _allocate_data(1, 0, 0, smt, output);

    fprintf(output, "section .rodata\n");
    _allocate_data(0, 1, 0, smt, output);
    
    map_foreach (str_info_t* si, &smt->s.strtb) {
        if (si->t != STR_INDEPENDENT) continue;
        fprintf(output, "_str_%li_ db ", si->id);
        char* data = si->value->body;
        while (*data) {
            fprintf(output, "%i,", *(data++));
        }

        fprintf(output, "0\n");
    }

    fprintf(output, "section .bss\n");
    _allocate_data(1, 0, 1, smt, output);

    fprintf(output, "section .text\n");
    map_foreach (func_info_t* fi, &smt->f.functb) {
        if (fi->flags.global)   fprintf(output, "global %s\n", fi->name->body);
        if (fi->flags.external) fprintf(output, "extern %s\n", fi->name->body);
    }

    return 1;
}
