#ifndef SYMTB_HELPER_H_
#define SYMTB_HELPER_H_
#include <std/map.h>
#include <symtab/symtab.h>
#include <hir/hir_types.h>
#include "reg_helper.h"

const char* format_tkntype(token_type_t t) {
    switch (t) {
        case ARRAY_TYPE_TOKEN: return "arr";
        case STR_TYPE_TOKEN:   return "str";
        case F64_TYPE_TOKEN:   return "f64";
        case F32_TYPE_TOKEN:   return "f32";
        case I64_TYPE_TOKEN:   return "i64";
        case I32_TYPE_TOKEN:   return "i32";
        case I16_TYPE_TOKEN:   return "i16";
        case I8_TYPE_TOKEN:    return "i8";
        case U64_TYPE_TOKEN:   return "u64";
        case U32_TYPE_TOKEN:   return "u32";
        case U16_TYPE_TOKEN:   return "u16";
        case U8_TYPE_TOKEN:    return "u8";
        default: return "?";
    }
}

void print_symtab(sym_table_t* smt) {
    printf("\n\n========== SYMTABLES ==========\n");
    map_iter_t it;

    if (!map_isempty(&smt->v.vartb)) printf("==========   VARS  ==========\n");
    map_iter_init(&smt->v.vartb, &it);
    variable_info_t* vi;
    while (map_iter_next(&it, (void**)&vi)) {
        printf("id: %i, %s, %s, s_id: %i", vi->v_id, vi->name, format_tkntype(vi->type), vi->s_id);
        if (vi->vmi.reg >= 0)         printf(", reg=%s", register_to_string(vi->vmi.reg + R11));
        else if (vi->vmi.offset >= 0) printf(", mem=[rbp - %i]", vi->vmi.offset);
        printf("\n");
    }

    if (!map_isempty(&smt->a.arrtb)) printf("==========   ARRS  ==========\n");
    map_iter_init(&smt->a.arrtb, &it);
    array_info_t* ai;
    while (map_iter_next(&it, (void**)&ai)) {
        printf("id: %i, %s x %i%s\n", ai->v_id, format_tkntype(ai->el_type), ai->size, ai->heap ? ", heap" : "");
    }

    if (!map_isempty(&smt->f.functb)) printf("==========  FUNCS  ==========\n");
    map_iter_init(&smt->f.functb, &it);
    func_info_t* fi;
    while (map_iter_next(&it, (void**)&fi)) {
        printf("id: %i, name: %s\n", fi->id, fi->name);
    }

    if (!map_isempty(&smt->s.strtb)) printf("========== STRINGS ==========\n");
    map_iter_init(&smt->s.strtb, &it);
    str_info_t* si;
    while (map_iter_next(&it, (void**)&si)) {
        printf("id: %i, val: %s, t=%i\n", si->id, si->value, si->t);
    }

    if (!map_isempty(&smt->m.allias)) printf("========== ALLIAS ==========\n");
    map_iter_init(&smt->m.allias, &it);
    allias_t* mi;
    while (map_iter_next(&it, (void**)&mi)) {
        printf("id: %i, owners: ", mi->v_id);
        set_iter_t sit;
        set_iter_init(&mi->owners, &sit);
        long own_id;
        while (set_iter_next(&sit, (void**)&own_id)) printf("%i ", own_id);
        printf("\n");
    }
}
#endif