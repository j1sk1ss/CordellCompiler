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
        case TMP_F64_TYPE_TOKEN:
        case F64_TYPE_TOKEN:   return "f64";
        case TMP_F32_TYPE_TOKEN:
        case F32_TYPE_TOKEN:   return "f32";
        case TMP_I64_TYPE_TOKEN:
        case I64_TYPE_TOKEN:   return "i64";
        case TMP_I32_TYPE_TOKEN:
        case I32_TYPE_TOKEN:   return "i32";
        case TMP_I16_TYPE_TOKEN:
        case I16_TYPE_TOKEN:   return "i16";
        case TMP_I8_TYPE_TOKEN:
        case I8_TYPE_TOKEN:    return "i8";
        case TMP_U64_TYPE_TOKEN:
        case U64_TYPE_TOKEN:   return "u64";
        case TMP_U32_TYPE_TOKEN:
        case U32_TYPE_TOKEN:   return "u32";
        case TMP_U16_TYPE_TOKEN:
        case U16_TYPE_TOKEN:   return "u16";
        case TMP_U8_TYPE_TOKEN:
        case U8_TYPE_TOKEN:    return "u8";
        default: return "?";
    }
}

void print_symtab(sym_table_t* smt) {
    printf("\n\n========== SYMTABLES ==========\n");

    if (!map_isempty(&smt->v.vartb)) printf("==========   VARS  ==========\n");
    map_foreach (variable_info_t* vi, &smt->v.vartb) {
        printf("id: %li, %s, %s, s_id: %i", vi->v_id, vi->name->body, format_tkntype(vi->type), vi->s_id);
        if (vi->vmi.reg >= 0)         printf(", reg=%s", register_to_string(vi->vmi.reg));
        else if (vi->vmi.offset >= 0) printf(", mem=[rbp - %li]", vi->vmi.offset);
        if (vi->vdi.defined)          printf(", value=%ld", vi->vdi.definition);
        printf("\n");
    }

    if (!map_isempty(&smt->a.arrtb)) printf("==========   ARRS  ==========\n");
    map_foreach (array_info_t* ai, &smt->a.arrtb) {
        printf("id: %li, %s x %li%s\n", ai->v_id, format_tkntype(ai->el_type), ai->size, ai->heap ? ", heap" : "");
    }

    if (!map_isempty(&smt->f.functb)) printf("==========  FUNCS  ==========\n");
    map_foreach (func_info_t* fi, &smt->f.functb) {
        printf(
            "%sid: %li, name: %s (virt: %s), ext=%i, glob=%i, used=%i\n", 
            fi->flags.entry ? "[ENTRY] " : "", fi->id, fi->name->body, fi->virt->body, fi->flags.external, fi->flags.global, fi->flags.used
        );
    }

    if (!map_isempty(&smt->s.strtb)) printf("========== STRINGS ==========\n");
    map_foreach (str_info_t* si, &smt->s.strtb) {
        printf("id: %li, val: %s, t=%i\n", si->id, si->value->body, si->t);
    }

    if (!map_isempty(&smt->m.allias)) printf("========== ALLIAS ==========\n");
    map_foreach (allias_t* mi, &smt->m.allias) {
        printf("id: %li, owners: ", mi->v_id);
        set_foreach (long own_id, &mi->owners) printf("%li ", own_id);
        printf("\n");
    }
}
#endif