#include <lir/lirgens/lirgens.h>

lir_subject_t* x86_64_format_variable(hir_subject_t* subj) {
    if (!subj) return NULL;
    switch (subj->t) {
        case HIR_F64NUMBER:
        case HIR_I64NUMBER:
        case HIR_U64NUMBER:
        case HIR_F32NUMBER:
        case HIR_I32NUMBER:
        case HIR_U32NUMBER:
        case HIR_I16NUMBER:
        case HIR_U16NUMBER:
        case HIR_I8NUMBER:
        case HIR_U8NUMBER:
        case HIR_NUMBER:   return LIR_SUBJ_NUMBER(subj->storage.num.value);

        case HIR_F64CONSTVAL:
        case HIR_I64CONSTVAL:
        case HIR_U64CONSTVAL:
        case HIR_F32CONSTVAL:
        case HIR_I32CONSTVAL:
        case HIR_U32CONSTVAL:
        case HIR_I16CONSTVAL:
        case HIR_U16CONSTVAL:
        case HIR_I8CONSTVAL:
        case HIR_U8CONSTVAL:
        case HIR_CONSTVAL: return LIR_SUBJ_CONST(subj->storage.cnst.value);
        
        case HIR_RAWASM:   return LIR_SUBJ_RAWASM(subj->storage.str.s_id);
        case HIR_STRING:   return LIR_SUBJ_STRING(subj->storage.str.s_id);
        
        case HIR_TMPVARF64: case HIR_TMPVARF32:
        case HIR_TMPVARSTR: case HIR_TMPVARARR: case HIR_TMPVARI64: case HIR_TMPVARU64:
        case HIR_TMPVARU32: case HIR_TMPVARI32: case HIR_TMPVARU16: case HIR_TMPVARI16: 
        case HIR_TMPVARU8:  case HIR_TMPVARI8:  case HIR_STKVARSTR: case HIR_STKVARARR: 
        case HIR_STKVARF64: case HIR_STKVARU64: case HIR_STKVARI64: case HIR_STKVARF32: 
        case HIR_STKVARU32: case HIR_STKVARI32: case HIR_STKVARU16: case HIR_STKVARI16: 
        case HIR_STKVARU8:  case HIR_STKVARI8:  case HIR_GLBVARSTR: case HIR_GLBVARARR: 
        case HIR_GLBVARF64: case HIR_GLBVARU64: case HIR_GLBVARI64: case HIR_GLBVARF32: 
        case HIR_GLBVARU32: case HIR_GLBVARI32: case HIR_GLBVARU16: case HIR_GLBVARI16: 
        case HIR_GLBVARU8:  case HIR_GLBVARI8: return LIR_SUBJ_VAR(subj->storage.var.v_id, HIR_get_type_size(subj->t));
        
        default: return NULL;
    }
}

int x86_64_store_var2var(lir_operation_t op, lir_ctx_t* ctx, hir_subject_t* dst, hir_subject_t* src) {
    LIR_BLOCK2(ctx, op, x86_64_format_variable(dst), x86_64_format_variable(src));
    return 1;
}
