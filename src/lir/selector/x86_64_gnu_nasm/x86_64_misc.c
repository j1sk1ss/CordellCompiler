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
