#include <lir/x86_64_gnu_nasm/x86_64_lirgen.h>

lir_subject_t* x86_64_format_variable(lir_ctx_t* ctx, hir_subject_t* subj, sym_table_t* smt) {
    if (!subj) return NULL;
    switch (subj->t) {
        case HIR_NUMBER:   return LIR_SUBJ_NUMBER(subj->storage.num.value);
        case HIR_CONSTVAL: return LIR_SUBJ_CONST(subj->storage.cnst.value);
        case HIR_STRING:   return LIR_SUBJ_STRING(subj->storage.str.s_id);
        case HIR_RAWASM:   return LIR_SUBJ_RAWASM(subj->storage.str.s_id);
        
        case HIR_TMPVARF64: case HIR_TMPVARF32:
        case HIR_TMPVARSTR: case HIR_TMPVARARR: case HIR_TMPVARI64: case HIR_TMPVARU64:
        case HIR_TMPVARU32: case HIR_TMPVARI32: case HIR_TMPVARU16: case HIR_TMPVARI16: 
        case HIR_TMPVARU8:  case HIR_TMPVARI8:  case HIR_STKVARSTR: case HIR_STKVARARR: 
        case HIR_STKVARF64: case HIR_STKVARU64: case HIR_STKVARI64: case HIR_STKVARF32: 
        case HIR_STKVARU32: case HIR_STKVARI32: case HIR_STKVARU16: case HIR_STKVARI16: 
        case HIR_STKVARU8:  case HIR_STKVARI8:  case HIR_GLBVARSTR: case HIR_GLBVARARR: 
        case HIR_GLBVARF64: case HIR_GLBVARU64: case HIR_GLBVARI64: case HIR_GLBVARF32: 
        case HIR_GLBVARU32: case HIR_GLBVARI32: case HIR_GLBVARU16: case HIR_GLBVARI16: 
        case HIR_GLBVARU8:  case HIR_GLBVARI8: {
            variable_info_t vi;
            if (VRTB_get_info_id(subj->storage.var.v_id, &vi, &smt->v)) {
                if (vi.glob) return LIR_SUBJ_GLVAR(subj->storage.var.v_id);
                if (!vi.vmi.allocated) {
                    long clr;
                    int vrsize = LIR_get_hirtype_size(subj->t);
                    if (
                        map_get(ctx->vars, subj->storage.var.v_id, (void**)&clr) && 
                        clr + 1 <= FREE_REGISTERS
                    ) vi.vmi.reg = clr;
                    else {
                        int vroff = stack_map_alloc(vrsize, &ctx->stk);
                        vi.vmi.offset = vroff;
                    }

                    vi.vmi.size = vrsize;
                    VRTB_update_memory(vi.v_id, vi.vmi.offset, vi.vmi.size, vi.vmi.reg, &smt->v);
                }

                if (vi.vmi.reg < 0) return LIR_SUBJ_OFF(vi.vmi.offset, vi.vmi.size);
                if (HIR_is_floattype(subj->t)) return LIR_SUBJ_REG(XMM0 + vi.vmi.reg, vi.vmi.size);
                return LIR_SUBJ_REG(R11 + vi.vmi.reg, vi.vmi.size);
            }
        }
        
        default: return NULL;
    }
}

/* Variable to register */
int x86_64_store_var_reg(lir_operation_t op, lir_ctx_t* ctx, hir_subject_t* subj, int reg, sym_table_t* smt) {
    LIR_BLOCK2(ctx, op, LIR_SUBJ_REG(reg, DEFAULT_TYPE_SIZE), x86_64_format_variable(ctx, subj, smt));
    return 1;
}

/* Variable from register */
int x86_64_load_var_reg(lir_operation_t op, lir_ctx_t* ctx, hir_subject_t* subj, int reg, sym_table_t* smt) {
    LIR_BLOCK2(ctx, op, x86_64_format_variable(ctx, subj, smt), LIR_SUBJ_REG(reg, LIR_get_hirtype_size(subj->t)));
    return 1;
}

int x86_64_reg_op(lir_ctx_t* ctx, int freg, int fs, int sreg, int ss, lir_operation_t op) {
    LIR_BLOCK2(ctx, op, LIR_SUBJ_REG(freg, fs), LIR_SUBJ_REG(sreg, ss));
    return 1;
}
