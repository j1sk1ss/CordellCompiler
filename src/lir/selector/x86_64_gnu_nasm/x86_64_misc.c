#include <lir/selector/x84_64_gnu_nasm.h>

lir_subject_t* create_tmp(lir_registers_t reg, lir_subject_t* src, sym_table_t* smt, int forced_size) {
    variable_info_t vi = { .vmi.offset = -1 };
    token_type_t vtype = TMP_TYPE_TOKEN;
    int vsize = forced_size < 0 ? src->size : forced_size;

    if (
        src->t == LIR_VARIABLE && 
        VRTB_get_info_id(src->storage.var.v_id, &vi, &smt->v)
    ) {
        vtype = vi.type;
        vsize = vi.vfs.ptr > 0 ? 8 : vsize;
    }
    
    symbol_id_t cpy = VRTB_add_info(NULL, vtype, NO_SYMBOL_ID, NULL, &smt->v);
    VRTB_update_memory(cpy, vi.vmi.offset, vsize, reg, FIELD_NO_CHANGE, &smt->v);
    return LIR_SUBJ_VAR(cpy, vsize);
}

int is_sign_type(lir_subject_t* s, sym_table_t* smt) {
    if (s->t != LIR_VARIABLE && s->t != LIR_GLVARIABLE) return 1;
    variable_info_t vi;
    if (!VRTB_get_info_id(s->storage.var.v_id, &vi, &smt->v)) return 1;
    switch (vi.type) {
        case U64_TYPE_TOKEN: case U32_TYPE_TOKEN:
        case U16_TYPE_TOKEN: case U8_TYPE_TOKEN: return 0;
        default: return 1;
    }
}

int is_simd_type(lir_subject_t* s, sym_table_t* smt) {
    if (s->t == LIR_NUMBER) return s->storage.num.is_float;
    if (s->t != LIR_VARIABLE && s->t != LIR_GLVARIABLE) return 0;
    variable_info_t vi;
    if (!VRTB_get_info_id(s->storage.var.v_id, &vi, &smt->v)) return 0;
    switch (vi.type) {
        case F64_TYPE_TOKEN: case F32_TYPE_TOKEN: return 1;
        default: return 0;
    }
}

lir_operation_t get_proper_mov(lir_subject_t* a, lir_subject_t* b, sym_table_t* smt, lir_operation_t base) {
    int to_float   = is_simd_type(a, smt);
    int from_float = is_simd_type(b, smt);

    if (to_float) {
        if (from_float) {
            if (b->size == 4 && a->size == 8)      return LIR_CVTSS2SD;
            else if (b->size == 8 && a->size == 4) return LIR_CVTSD2SS;
            else return base;
        } 
        else {
            if (a->size <= 4) return (b->size == 4) ? LIR_CVTTSS2SI : LIR_CVTTSD2SI;
            else return (b->size == 4) ? LIR_CVTTSS2SI : LIR_CVTTSD2SI;
        }
    }
    else {
        int from_sign = is_sign_type(b, smt);
        int from_num  = b->t == LIR_NUMBER;
        if (from_num) return base;
        
        if (from_float) {
            if (b->size == 4) return LIR_CVTTSS2SI;
            else return LIR_CVTTSD2SI;
        }
        else {
            if (a->size <= b->size) {
                b->size = a->size;
                return base;
            }
            else {
                if (b->size == 4 && a->size == 8) return from_sign ? LIR_MOVSXD : base;
                else return from_sign ? LIR_MOVSX : LIR_MOVZX;
            }
        }
    }

    return base;
}
