#include <hir/hir_types.h>

int HIR_get_type_size(hir_subject_type_t t) {
    switch (t) {
        case HIR_TMPVARI8:  case HIR_TMPVARU8:
        case HIR_STKVARI8:  case HIR_STKVARU8:
        case HIR_GLBVARI8:  case HIR_GLBVARU8:  return 1;
        case HIR_TMPVARI16: case HIR_TMPVARU16:
        case HIR_STKVARI16: case HIR_STKVARU16:
        case HIR_GLBVARI16: case HIR_GLBVARU16: return 2;
        case HIR_TMPVARI32: case HIR_TMPVARU32: 
        case HIR_TMPVARF32: case HIR_STKVARI32: 
        case HIR_STKVARU32: case HIR_STKVARF32:
        case HIR_GLBVARI32: case HIR_GLBVARU32: 
        case HIR_GLBVARF32: return 4;
        default:            return 8;
    }
}

int HIR_is_tmptype(hir_subject_type_t t) {
    switch (t) {
        case HIR_TMPVARI8: 
        case HIR_TMPVARU8:  
        case HIR_TMPVARI16: 
        case HIR_TMPVARU16: 
        case HIR_TMPVARI32: 
        case HIR_TMPVARU32: 
        case HIR_TMPVARF32: 
        case HIR_TMPVARI64: 
        case HIR_TMPVARU64: 
        case HIR_TMPVARF64: return 1;
        default:            return 0;
    }
}

int HIR_is_vartype(hir_subject_type_t t) {
    switch (t) {
        case HIR_TMPVARI8:  case HIR_TMPVARU8:  
        case HIR_TMPVARI16: case HIR_TMPVARU16: 
        case HIR_TMPVARI32: case HIR_TMPVARU32: 
        case HIR_TMPVARF32: case HIR_TMPVARI64: 
        case HIR_TMPVARU64: case HIR_TMPVARF64: 
        case HIR_STKVARI8:  case HIR_STKVARU8:  
        case HIR_STKVARI16: case HIR_STKVARU16: 
        case HIR_STKVARI32: case HIR_STKVARU32: 
        case HIR_STKVARF32: case HIR_STKVARI64: 
        case HIR_STKVARU64: case HIR_STKVARF64: 
        case HIR_GLBVARI8:  case HIR_GLBVARU8:  
        case HIR_GLBVARI16: case HIR_GLBVARU16: 
        case HIR_GLBVARI32: case HIR_GLBVARU32: 
        case HIR_GLBVARF32: case HIR_GLBVARI64: 
        case HIR_GLBVARU64: case HIR_GLBVARF64: 
        case HIR_STKVARSTR: case HIR_GLBVARSTR:
        case HIR_STKVARARR: case HIR_GLBVARARR: return 1;
        default: return 0;
    }
}

int HIR_defined_type(hir_subject_type_t t) {
    switch (t) {
        case HIR_I64NUMBER:
        case HIR_I32NUMBER:
        case HIR_I16NUMBER:
        case HIR_I8NUMBER:
        case HIR_NUMBER:
        case HIR_I64CONSTVAL:
        case HIR_I32CONSTVAL:
        case HIR_I16CONSTVAL:
        case HIR_I8CONSTVAL:
        case HIR_CONSTVAL: return 1;
        default: return 0;
    }
}

hir_subject_type_t HIR_promote_types(hir_subject_type_t a, hir_subject_type_t b) {
    int size_a = HIR_get_type_size(a);
    int size_b = HIR_get_type_size(b);

    if (HIR_is_float(a) || HIR_is_float(b)) {
        int max_size = size_a > size_b ? size_a : size_b;
        return max_size <= 4 ? HIR_TMPVARF32 : HIR_TMPVARF64; 
    }

    int issigned_a = (a == HIR_TMPVARI8 || a == HIR_TMPVARI16 || a == HIR_TMPVARI32 || a == HIR_TMPVARI64 ||
                      a == HIR_STKVARI8 || a == HIR_STKVARI16 || a == HIR_STKVARI32 || a == HIR_STKVARI64 ||
                      a == HIR_GLBVARI8 || a == HIR_GLBVARI16 || a == HIR_GLBVARI32 || a == HIR_GLBVARI64);
    int issigned_b = (b == HIR_TMPVARI8 || b == HIR_TMPVARI16 || b == HIR_TMPVARI32 || b == HIR_TMPVARI64 ||
                      b == HIR_STKVARI8 || b == HIR_STKVARI16 || b == HIR_STKVARI32 || b == HIR_STKVARI64 ||
                      b == HIR_GLBVARI8 || b == HIR_GLBVARI16 || b == HIR_GLBVARI32 || b == HIR_GLBVARI64);
    int max_size = size_a > size_b ? size_a : size_b;
    if (issigned_a != issigned_b && size_a == size_b) {
        switch (max_size) {
            case 1:  return HIR_TMPVARU8;
            case 2:  return HIR_TMPVARU16;
            case 4:  return HIR_TMPVARU32;
            default: return HIR_TMPVARU64;
        }
    }

    if (size_a >= size_b) return a;
    else return b;
}

hir_subject_type_t HIR_get_tmp_type(hir_subject_type_t t) {
    switch (t) {
        case HIR_TMPVARI8: 
        case HIR_STKVARI8: 
        case HIR_GLBVARI8: return HIR_TMPVARI8;
        
        case HIR_TMPVARI16: 
        case HIR_STKVARI16: 
        case HIR_GLBVARI16: return HIR_TMPVARI16;
        
        case HIR_TMPVARI32: 
        case HIR_STKVARI32: 
        case HIR_GLBVARI32: return HIR_TMPVARI32;
        
        case HIR_TMPVARI64: 
        case HIR_STKVARI64: 
        case HIR_GLBVARI64: return HIR_TMPVARI64;

        case HIR_TMPVARU8: 
        case HIR_STKVARU8: 
        case HIR_GLBVARU8: return HIR_TMPVARU8;
        
        case HIR_TMPVARU16: 
        case HIR_STKVARU16: 
        case HIR_GLBVARU16: return HIR_TMPVARU16;
        
        case HIR_TMPVARU32: 
        case HIR_STKVARU32: 
        case HIR_GLBVARU32: return HIR_TMPVARU32;
        
        case HIR_TMPVARU64: 
        case HIR_STKVARU64: 
        case HIR_GLBVARU64: return HIR_TMPVARU64;

        case HIR_TMPVARF32: 
        case HIR_STKVARF32: 
        case HIR_GLBVARF32: return HIR_TMPVARF32;
        
        case HIR_TMPVARF64: 
        case HIR_STKVARF64: 
        case HIR_GLBVARF64: return HIR_TMPVARF64;

        default: return HIR_TMPVARI64;
    }
}

hir_subject_type_t HIR_get_tmptype_tkn(token_t* token, int ptr) {
    if (!token) return HIR_TMPVARI64;
    int bitness  = TKN_variable_bitness(token, ptr);
    int isfloat  = TKN_is_float(token);
    int issigned = TKN_issign(token) || !ptr;
    if (!isfloat) {
        switch (bitness) {
            case 8:  return issigned ? HIR_TMPVARI8  : HIR_TMPVARU8;
            case 16: return issigned ? HIR_TMPVARI16 : HIR_TMPVARU16;
            case 32: return issigned ? HIR_TMPVARI32 : HIR_TMPVARU32;
            default: return issigned ? HIR_TMPVARI64 : HIR_TMPVARU64;
        }
    }

    switch (bitness) {
        case 32: return HIR_TMPVARF32;
        default: return HIR_TMPVARF64;
    }
}

hir_subject_type_t _get_glbtype(int bitness, int isfloat, int issigned) {
    if (!isfloat) {
        switch (bitness) {
            case 8:  return issigned ? HIR_GLBVARI8 : HIR_GLBVARU8;
            case 16: return issigned ? HIR_GLBVARI16 : HIR_GLBVARU16;
            case 32: return issigned ? HIR_GLBVARI32 : HIR_GLBVARU32;
            default: return issigned ? HIR_GLBVARI64 : HIR_GLBVARU64;
        }
    }

    switch (bitness) {
        case 32: return HIR_GLBVARF32;
        default: return HIR_GLBVARF64;
    }
}

hir_subject_type_t HIR_get_stktype(variable_info_t* vi) {
    if (!vi) return HIR_STKVARI64;
    token_t tmptkn = { .t_type = vi->type, .flags = { .ptr = vi->vfs.ptr, .ro = vi->vfs.ro, .glob = vi->vfs.glob } };
    int bitness  = TKN_variable_bitness(&tmptkn, 1);
    int isfloat  = TKN_is_float(&tmptkn);
    int issigned = TKN_issign(&tmptkn);
    int isarr    = vi->type == ARR_VARIABLE_TOKEN;
    int isstr    = vi->type == STR_VARIABLE_TOKEN;

    if (!TKN_instack(&tmptkn)) {
        if (isarr) return HIR_GLBVARARR;
        if (isstr) return HIR_GLBVARSTR;
        return _get_glbtype(bitness, isfloat, issigned);
    }
    
    if (isarr) return HIR_STKVARARR;
    if (isstr) return HIR_STKVARSTR;
    if (!isfloat) {
        switch (bitness) {
            case 8:  return issigned ? HIR_STKVARI8 : HIR_STKVARU8;
            case 16: return issigned ? HIR_STKVARI16 : HIR_STKVARU16;
            case 32: return issigned ? HIR_STKVARI32 : HIR_STKVARU32;
            default: return issigned ? HIR_STKVARI64 : HIR_STKVARU64;
        }
    }

    switch (bitness) {
        case 32: return HIR_STKVARF32;
        default: return HIR_STKVARF64;
    }
}

hir_subject_type_t HIR_get_token_stktype(token_t* tkn, int ptr) {
    variable_info_t vi = { 
        .type = tkn->t_type, 
        .vfs  = {
            .ptr  = ptr, 
            .glob = tkn->flags.glob, 
            .ro   = tkn->flags.ro 
        }
    };

    return HIR_get_stktype(&vi);
}

int HIR_isjmp(hir_operation_t op) {
    if (
        op == HIR_JMP   ||
        op == HIR_IFOP2
    ) return 1;
    return 0;
}

int HIR_issyst(hir_operation_t op) {
    if (
        op == HIR_MKSCOPE  ||
        op == HIR_ENDSCOPE ||
        op == HIR_STEND    ||
        op == HIR_FEND
    ) return 1;
    return 0;    
}

int HIR_isterm(hir_operation_t op) {
    if (
        op == HIR_EXITOP ||
        op == HIR_FRET
    ) return 1;
    return 0;
}

int HIR_funccall(hir_operation_t op) {
    switch (op) {
        case HIR_FCLL:
        case HIR_STORE_FCLL:
        case HIR_ECLL:
        case HIR_STORE_ECLL: return 1;
        default: return 0;
    }
}

int HIR_commutative_op(hir_operation_t op) {
    switch (op) {
        case HIR_iCMP:
        case HIR_iOR:
        case HIR_iAND:
        case HIR_iADD:
        case HIR_iMUL: return 1;
        default: return 0;
    }
}

int HIR_sideeffect_op(hir_operation_t op) {
    switch (op) {
        case HIR_JMP:
        case HIR_PHI:
        case HIR_MKLB:
        case HIR_FRET:
        case HIR_ECLL:
        case HIR_FCLL:
        case HIR_SYSC:
        case HIR_STORE:
        case HIR_EXITOP:
        case HIR_MKSCOPE:
        case HIR_ENDSCOPE:
        case HIR_STORE_ECLL:
        case HIR_STORE_FCLL:
        case HIR_STORE_SYSC:
        case HIR_BREAKPOINT:
        case HIR_PHI_PREAMBLE:
        case HIR_IFOP2: return 1;
        default: return 0;
    }
}

int HIR_writeop(hir_operation_t op) {
    switch (op) {
        case HIR_ARRDECL:
        case HIR_REF:
        case HIR_FARGLD:
        case HIR_STARGLD:
        case HIR_STORE_ECLL:
        case HIR_STORE_FCLL:
        case HIR_STORE_SYSC:
        case HIR_TF64:
        case HIR_TF32:
        case HIR_TI64:
        case HIR_TI32:
        case HIR_TI16:
        case HIR_TI8:
        case HIR_TU64:
        case HIR_TU32:
        case HIR_TU16:
        case HIR_TU8:
        case HIR_iADD:
        case HIR_iSUB:
        case HIR_iMUL:
        case HIR_iDIV:
        case HIR_iMOD:
        case HIR_iLRG:
        case HIR_iLGE:
        case HIR_iLWR:
        case HIR_iLRE:
        case HIR_iCMP:
        case HIR_iNMP:
        case HIR_iAND:
        case HIR_iOR:
        case HIR_iBLFT:
        case HIR_iBRHT:
        case HIR_bAND:
        case HIR_bOR:
        case HIR_bXOR:
        case HIR_PHI:
        case HIR_STORE: return 1;
        default:        return 0;
    }
}

hir_operation_t HIR_convop(hir_subject_type_t t) {
    switch (t) {
        case HIR_F64NUMBER:
        case HIR_TMPVARF64: case HIR_STKVARF64: case HIR_GLBVARF64: return HIR_TF64;
        case HIR_F32NUMBER:
        case HIR_TMPVARF32: case HIR_STKVARF32: case HIR_GLBVARF32: return HIR_TF32;
        case HIR_TMPVARI64: case HIR_STKVARI64: case HIR_GLBVARI64: return HIR_TI64;
        case HIR_TMPVARI32: case HIR_STKVARI32: case HIR_GLBVARI32: return HIR_TI32;
        case HIR_TMPVARI16: case HIR_STKVARI16: case HIR_GLBVARI16: return HIR_TI16;
        case HIR_TMPVARI8:  case HIR_STKVARI8:  case HIR_GLBVARI8:  return HIR_TI8; 
        case HIR_TMPVARU64: case HIR_STKVARU64: case HIR_GLBVARU64: return HIR_TU64;
        case HIR_TMPVARU32: case HIR_STKVARU32: case HIR_GLBVARU32: return HIR_TU32;
        case HIR_TMPVARU16: case HIR_STKVARU16: case HIR_GLBVARU16: return HIR_TU16;
        case HIR_TMPVARU8:  case HIR_STKVARU8:  case HIR_GLBVARU8:  return HIR_TU8; 
        default: return HIR_STORE;
    }
}

int HIR_similar_type(hir_subject_type_t a, hir_subject_type_t b) {
    return HIR_get_tmp_type(a) == HIR_get_tmp_type(b);
}

token_type_t HIR_get_tmptkn_type(hir_subject_type_t t) {
    switch (t) {
        case HIR_GLBVARF64:
        case HIR_STKVARF64:
        case HIR_TMPVARF64: return TMP_F64_TYPE_TOKEN;

        case HIR_GLBVARF32:
        case HIR_STKVARF32:
        case HIR_TMPVARF32: return TMP_F32_TYPE_TOKEN;

        case HIR_GLBVARI64:
        case HIR_STKVARI64:
        case HIR_TMPVARI64: return TMP_I64_TYPE_TOKEN;
        
        case HIR_GLBVARI32:
        case HIR_STKVARI32:
        case HIR_TMPVARI32: return TMP_I32_TYPE_TOKEN;

        case HIR_GLBVARI16:
        case HIR_STKVARI16:
        case HIR_TMPVARI16: return TMP_I16_TYPE_TOKEN;

        case HIR_GLBVARI8:
        case HIR_STKVARI8:
        case HIR_TMPVARI8:  return TMP_I8_TYPE_TOKEN;

        case HIR_GLBVARU64:
        case HIR_STKVARU64:
        case HIR_TMPVARU64: return TMP_U64_TYPE_TOKEN;

        case HIR_GLBVARU32:
        case HIR_STKVARU32:
        case HIR_TMPVARU32: return TMP_U32_TYPE_TOKEN;

        case HIR_GLBVARU16:
        case HIR_STKVARU16:
        case HIR_TMPVARU16: return TMP_U16_TYPE_TOKEN;

        case HIR_TMPVARU8:
        case HIR_STKVARU8:
        case HIR_GLBVARU8: return TMP_U8_TYPE_TOKEN; 
        default: return TMP_F64_TYPE_TOKEN;
    }
}

int HIR_is_float(hir_subject_type_t t) {
    switch (t) {
        case HIR_GLBVARF64:
        case HIR_STKVARF64:
        case HIR_TMPVARF64:
        case HIR_GLBVARF32:
        case HIR_STKVARF32:
        case HIR_TMPVARF32: return 1;
        default: return 0;
    }
}
