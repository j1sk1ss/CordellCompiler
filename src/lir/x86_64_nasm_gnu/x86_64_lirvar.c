#include <lir/x86_64_gnu_nasm/x86_64_lirgen.h>

lir_subject_t* LIR_format_variable(hir_subject_t* subj, sym_table_t* smt) {
    if (!subj) return NULL;
    switch (subj->t) {
        case HIR_NUMBER:   return LIR_SUBJ_NUMBER(subj->storage.num.value);
        case HIR_CONSTVAL: return LIR_SUBJ_CONST(subj->storage.cnst.value);
        
        case HIR_TMPVARF64: case HIR_TMPVARF32: return LIR_SUBJ_REG(XMM0, HIR_get_type_size(subj->t));
        case HIR_TMPVARSTR: case HIR_TMPVARARR: case HIR_TMPVARI64: case HIR_TMPVARU64:
        case HIR_TMPVARU32: case HIR_TMPVARI32: case HIR_TMPVARU16: case HIR_TMPVARI16: 
        case HIR_TMPVARU8:  case HIR_TMPVARI8: return LIR_SUBJ_REG(RAX, HIR_get_type_size(subj->t));

        case HIR_STKVARSTR: case HIR_STKVARARR: case HIR_STKVARF64: case HIR_STKVARU64:
        case HIR_STKVARI64: case HIR_STKVARF32: case HIR_STKVARU32: case HIR_STKVARI32:
        case HIR_STKVARU16: case HIR_STKVARI16: case HIR_STKVARU8:  case HIR_STKVARI8:
        case HIR_GLBVARSTR: case HIR_GLBVARARR: case HIR_GLBVARF64: case HIR_GLBVARU64:
        case HIR_GLBVARI64: case HIR_GLBVARF32: case HIR_GLBVARU32: case HIR_GLBVARI32:
        case HIR_GLBVARU16: case HIR_GLBVARI16: case HIR_GLBVARU8:  case HIR_GLBVARI8: {
            variable_info_t vi;
            if (VRTB_get_info_id(subj->storage.var.v_id, &vi, &smt->v)) {
                if (vi.p_id >= 0 && !VRTB_get_info_id(vi.p_id, &vi, &smt->v)) return NULL;
                token_t tmptkn = { .t_type = vi.type };
                if (!vi.glob) return LIR_SUBJ_OFF(vi.offset, VRS_variable_bitness(&tmptkn, vi.ptr) / DEFAULT_TYPE_SIZE);
                else return LIR_SUBJ_GLVAR(subj->storage.var.v_id);
            }
        }
        
        default: return NULL;
    }
}

/* Variable to register */
int LIR_store_var_reg(lir_operation_t op, lir_ctx_t* ctx, hir_subject_t* subj, int reg, sym_table_t* smt) {
    LIR_BLOCK2(ctx, op, LIR_SUBJ_REG(reg, LIR_get_hirtype_size(subj->t)), LIR_format_variable(subj, smt));
    return 1;
}

/* Variable from register */
int LIR_load_var_reg(lir_operation_t op, lir_ctx_t* ctx, hir_subject_t* subj, int reg, sym_table_t* smt) {
    LIR_BLOCK2(ctx, op, LIR_format_variable(subj, smt), LIR_SUBJ_REG(reg, LIR_get_hirtype_size(subj->t)));
    return 1;
}

int LIR_reg_op(lir_ctx_t* ctx, int freg, int sreg, lir_operation_t op) {
    LIR_BLOCK2(ctx, op, LIR_SUBJ_REG(freg, DEFAULT_TYPE_SIZE), LIR_SUBJ_REG(sreg, DEFAULT_TYPE_SIZE));
    return 1;
}
