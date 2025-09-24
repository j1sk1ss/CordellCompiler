#include <hir/hir_types.h>

int HIR_get_type_size(hir_subject_type_t t) {
    switch (t) {
        case HIR_TMPVARI8: 
        case HIR_TMPVARU8:  return 1;
        case HIR_TMPVARI16: 
        case HIR_TMPVARU16: return 2;
        case HIR_TMPVARI32: 
        case HIR_TMPVARU32: 
        case HIR_TMPVARF32: return 4;
        case HIR_TMPVARI64: 
        case HIR_TMPVARU64: 
        case HIR_TMPVARF64: return 8;
        case HIR_STKVARI8: 
        case HIR_STKVARU8:  return 1;
        case HIR_STKVARI16: 
        case HIR_STKVARU16: return 2;
        case HIR_STKVARI32: 
        case HIR_STKVARU32: 
        case HIR_STKVARF32: return 4;
        case HIR_STKVARI64: 
        case HIR_STKVARU64: 
        case HIR_STKVARF64: return 8;
        case HIR_GLBVARI8: 
        case HIR_GLBVARU8:  return 1;
        case HIR_GLBVARI16: 
        case HIR_GLBVARU16: return 2;
        case HIR_GLBVARI32: 
        case HIR_GLBVARU32: 
        case HIR_GLBVARF32: return 4;
        case HIR_GLBVARI64: 
        case HIR_GLBVARU64: 
        case HIR_GLBVARF64: return 8;
        case HIR_STKVARSTR:
        case HIR_GLBVARSTR:
        case HIR_STKVARARR:
        case HIR_GLBVARARR: return 8;
        default:            return 8;
    }
}

int HIR_is_vartype(hir_subject_type_t t) {
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
        case HIR_TMPVARF64: 
        case HIR_STKVARI8: 
        case HIR_STKVARU8:  
        case HIR_STKVARI16: 
        case HIR_STKVARU16: 
        case HIR_STKVARI32: 
        case HIR_STKVARU32: 
        case HIR_STKVARF32: 
        case HIR_STKVARI64: 
        case HIR_STKVARU64: 
        case HIR_STKVARF64: 
        case HIR_GLBVARI8: 
        case HIR_GLBVARU8:  
        case HIR_GLBVARI16: 
        case HIR_GLBVARU16: 
        case HIR_GLBVARI32: 
        case HIR_GLBVARU32: 
        case HIR_GLBVARF32: 
        case HIR_GLBVARI64: 
        case HIR_GLBVARU64: 
        case HIR_GLBVARF64: 
        case HIR_STKVARSTR:
        case HIR_GLBVARSTR:
        case HIR_STKVARARR:
        case HIR_GLBVARARR: return 1;
        default:            return 0;
    }
}

hir_subject_type_t HIR_promote_types(hir_subject_type_t a, hir_subject_type_t b) {
    int size_a = HIR_get_type_size(a);
    int size_b = HIR_get_type_size(b);

    int isfloat_a = (a == HIR_TMPVARF32 || a == HIR_TMPVARF64 ||
                     a == HIR_STKVARF32 || a == HIR_STKVARF64 ||
                     a == HIR_GLBVARF32 || a == HIR_GLBVARF64);
    int isfloat_b = (b == HIR_TMPVARF32 || b == HIR_TMPVARF64 ||
                     b == HIR_STKVARF32 || b == HIR_STKVARF64 ||
                     b == HIR_GLBVARF32 || b == HIR_GLBVARF64);

    if (isfloat_a || isfloat_b) {
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

hir_subject_type_t HIR_get_tmptype_tkn(token_t* token) {
    if (!token) return HIR_TMPVARI64;
    int bitness  = VRS_variable_bitness(token, 1);
    int isfloat  = VRS_is_float(token);
    int issigned = VRS_issign(token);
    if (!isfloat) {
        switch (bitness) {
            case 8:  return issigned ? HIR_TMPVARI8 : HIR_TMPVARU8;
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

hir_subject_type_t HIR_get_stktype(token_t* token) {
    if (!token) return HIR_STKVARI64;
    int bitness  = VRS_variable_bitness(token, 1);
    int isfloat  = VRS_is_float(token);
    int issigned = VRS_issign(token);
    int isarr    = token->t_type == ARR_VARIABLE_TOKEN;
    int isstr    = token->t_type == STR_VARIABLE_TOKEN;

    if (!VRS_instack(token)) {
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

int HIR_isleader(hir_operation_t op) {
    if (
        op == HIR_FDCL ||
        op == HIR_MKLB
    ) return 1;
    return 0;
}

int HIR_isjmp(hir_operation_t op) {
    if (
        op == HIR_FCLL    ||
        op == HIR_JMP     ||
        op == HIR_IFOP    ||
        op == HIR_IFCPOP  ||
        op == HIR_IFLGEOP ||
        op == HIR_IFLGOP  ||
        op == HIR_IFLWEOP ||
        op == HIR_IFLWOP  ||
        op == HIR_IFNCPOP ||
        op == HIR_FRET
    ) return 1;
    return 0;
}

int HIR_iscondjmp(hir_operation_t op) {
    if (
        op == HIR_IFOP    ||
        op == HIR_IFCPOP  ||
        op == HIR_IFLGEOP ||
        op == HIR_IFLGOP  ||
        op == HIR_IFLWEOP ||
        op == HIR_IFLWOP  ||
        op == HIR_IFNCPOP
    ) return 1;
    return 0;
}
