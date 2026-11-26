#ifndef CPL_DISABLE_I386_GNU_NASM
#include <lir/selector/i386_gnu_nasm.h>

lir_subject_t* i386_gnu_nasm_create_tmp(lir_registers_t reg, lir_subject_t* src, sym_table_t* smt, int forced_size) {
    token_type_t vtype = TMP_TYPE_TOKEN;
    int vsize = forced_size < 0 ? src->size : forced_size;
    basic_object_info_t flags = EMPTY_BASIC_FLAGS;
    if (i386_gnu_nasm_is_simd_type(src, smt)) vtype = vsize == 4 ? TMP_F32_TYPE_TOKEN : TMP_F64_TYPE_TOKEN;
    if (vsize > 4 && !(reg >= XMM0 && reg <= XMM7)) vsize = 4;

    variable_info_t vi;
    if (
        src->t == LIR_VARIABLE && 
        VRTB_get_info_id(src->storage.var.v_id, &vi, &smt->v)
    ) {
        vtype = vi.type;
        flags.ptr = vi.vfs.ptr;
        if (forced_size < 0 && vi.vfs.ptr > 0) vsize = 4;
    }
    
    symbol_id_t cpy = VRTB_add_info(NULL, vtype, NO_SYMBOL_ID, flags, &smt->v);
    VRTB_update_memory(cpy, FIELD_NO_CHANGE, vsize, reg, FIELD_NO_CHANGE, &smt->v);
    lir_subject_t* new = LIR_SUBJ_VAR(cpy, vsize);
    new->dsize = src->dsize;
    return new;
}

int i386_gnu_nasm_is_sign_type(lir_subject_t* s, sym_table_t* smt) {
    if (s->t != LIR_VARIABLE && s->t != LIR_GLVARIABLE) return 1;
    variable_info_t vi;
    if (!VRTB_get_info_id(s->storage.var.v_id, &vi, &smt->v)) return 1;
    switch (vi.type) {
        case TMP_U64_TYPE_TOKEN: case TMP_U32_TYPE_TOKEN:
        case TMP_U16_TYPE_TOKEN: case TMP_U8_TYPE_TOKEN:
        case U64_TYPE_TOKEN:     case U32_TYPE_TOKEN:
        case U16_TYPE_TOKEN:     case U8_TYPE_TOKEN: return 0;
        default: return 1;
    }
}

int i386_gnu_nasm_is_simd_type(lir_subject_t* s, sym_table_t* smt) {
    return LIR_is_float_subject(s, smt);
}

lir_operation_t i386_gnu_nasm_get_proper_mov(lir_subject_t* a, lir_subject_t* b, sym_table_t* smt, lir_operation_t base) {
    int to_float   = i386_gnu_nasm_is_simd_type(a, smt);
    int from_float = i386_gnu_nasm_is_simd_type(b, smt);

    if (to_float) {
        if (from_float) {
            if (b->size == 4 && a->size == 8)      return LIR_CVTSS2SD;
            else if (b->size == 8 && a->size == 4) return LIR_CVTSD2SS;
            return LIR_format_float_operation(base, 1);
        } 
        else {
            return a->size == 4 ? LIR_CVTSI2SS : LIR_CVTSI2SD;
        }
    }
    else {
        int from_sign = i386_gnu_nasm_is_sign_type(b, smt);
        int from_num  = LIR_is_defined_type(b->t);
        if (from_float) {
            if (b->size == 4) return LIR_CVTTSS2SI;
            else return LIR_CVTTSD2SI;
        }
        else {
            if (from_num) return base;
            if (a->size <= b->size) {
                b->size = a->size;
                return LIR_iMOV;
            }
            else {
                if (b->size == 4 && a->size == 8) return from_sign ? LIR_MOVSXD : base;
                else return from_sign ? LIR_MOVSX : LIR_MOVZX;
            }
        }
    }

    return base;
}
#endif
