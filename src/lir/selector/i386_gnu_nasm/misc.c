#include <lir/selector/i386_gnu_nasm.h>

lir_subject_t* i386_gnu_nasm_create_tmp(lir_registers_t reg, lir_subject_t* src, sym_table_t* smt, int forced_size) {
    token_type_t vtype = TMP_TYPE_TOKEN;
    int vsize = forced_size < 0 ? src->size : forced_size;
    if (vsize > 4) vsize = 4;

    variable_info_t vi;
    if (
        src->t == LIR_VARIABLE && 
        VRTB_get_info_id(src->storage.var.v_id, &vi, &smt->v)
    ) {
        vtype = vi.type;
        if (forced_size < 0 && vi.vfs.ptr > 0) vsize = 4;
    }
    
    symbol_id_t cpy = VRTB_add_info(NULL, vtype, NO_SYMBOL_ID, NULL, &smt->v);
    VRTB_update_memory(cpy, FIELD_NO_CHANGE, vsize, reg, FIELD_NO_CHANGE, &smt->v);
    lir_subject_t* new = LIR_SUBJ_VAR(cpy, vsize);
    new->dsize = src->dsize;
    return new;
}

static int _is_f64_type(lir_subject_t* s, sym_table_t* smt) {
    if (s->t == LIR_NUMBER) return s->storage.num.is_float && s->size == 4;
    if (s->t != LIR_VARIABLE && s->t != LIR_GLVARIABLE) return 0;
    variable_info_t vi;
    if (!VRTB_get_info_id(s->storage.var.v_id, &vi, &smt->v)) return 0;
    switch (vi.type) {
        case TMP_F64_TYPE_TOKEN:
        case F64_TYPE_TOKEN: return 1;
        default: return 0;
    }
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
    if (s->t == LIR_NUMBER) return s->storage.num.is_float;
    if (s->t != LIR_VARIABLE && s->t != LIR_GLVARIABLE) return 0;
    variable_info_t vi;
    if (!VRTB_get_info_id(s->storage.var.v_id, &vi, &smt->v)) return 0;
    switch (vi.type) {
        case TMP_F64_TYPE_TOKEN: case TMP_F32_TYPE_TOKEN:
        case F64_TYPE_TOKEN:     case F32_TYPE_TOKEN: return 1;
        default: return 0;
    }
}

lir_operation_t i386_gnu_nasm_get_proper_mov(lir_subject_t* a, lir_subject_t* b, sym_table_t* smt, lir_operation_t base) {
    int from_float = i386_gnu_nasm_is_simd_type(b, smt);
    int from_f64   = _is_f64_type(b, smt);
    if (i386_gnu_nasm_is_simd_type(a, smt)) {
        int to_f64 = _is_f64_type(a, smt);
        if (!from_float) return to_f64 ? LIR_CVTSI2SD : LIR_CVTSI2SS;
        else {
            if (!from_f64 && to_f64) return LIR_CVTSS2SD;
            if (from_f64 && !to_f64) return LIR_CVTSD2SS;
            return base;
        }
    }
    else {
        if (b->t == LIR_NUMBER) return base;
        if (from_float) return from_f64 ? LIR_CVTTSD2SI : LIR_CVTTSS2SI;
        else {
            if (a->size > b->size) return i386_gnu_nasm_is_sign_type(b, smt) ? LIR_MOVSX : LIR_MOVZX;
            else {
                b->size = a->size;
                return LIR_iMOV;
            }
        }
    }

    return base;
}
