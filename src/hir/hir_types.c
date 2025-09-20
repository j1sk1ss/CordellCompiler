#include <hir/hir_types.h>

int HIR_get_type_size(hir_subject_type_t t) {
    switch (t) {
        case TMPVARI8: 
        case TMPVARU8:  return 1;
        case TMPVARI16: 
        case TMPVARU16: return 2;
        case TMPVARI32: 
        case TMPVARU32: 
        case TMPVARF32: return 4;
        case TMPVARI64: 
        case TMPVARU64: 
        case TMPVARF64: return 8;
        case STKVARI8: 
        case STKVARU8:  return 1;
        case STKVARI16: 
        case STKVARU16: return 2;
        case STKVARI32: 
        case STKVARU32: 
        case STKVARF32: return 4;
        case STKVARI64: 
        case STKVARU64: 
        case STKVARF64: return 8;
        case GLBVARI8: 
        case GLBVARU8:  return 1;
        case GLBVARI16: 
        case GLBVARU16: return 2;
        case GLBVARI32: 
        case GLBVARU32: 
        case GLBVARF32: return 4;
        case GLBVARI64: 
        case GLBVARU64: 
        case GLBVARF64: return 8;
        case STKVARSTR:
        case GLBVARSTR:
        case STKVARARR:
        case GLBVARARR: return 8;
        default:        return 8;
    }
}

hir_subject_type_t HIR_promote_types(hir_subject_type_t a, hir_subject_type_t b) {
    int size_a = HIR_get_type_size(a);
    int size_b = HIR_get_type_size(b);

    int isfloat_a = (a == TMPVARF32 || a == TMPVARF64 ||
                     a == STKVARF32 || a == STKVARF64 ||
                     a == GLBVARF32 || a == GLBVARF64);
    int isfloat_b = (b == TMPVARF32 || b == TMPVARF64 ||
                     b == STKVARF32 || b == STKVARF64 ||
                     b == GLBVARF32 || b == GLBVARF64);

    if (isfloat_a || isfloat_b) {
        int max_size = size_a > size_b ? size_a : size_b;
        return max_size <= 4 ? TMPVARF32 : TMPVARF64; 
    }

    int issigned_a = (a == TMPVARI8 || a == TMPVARI16 || a == TMPVARI32 || a == TMPVARI64 ||
                      a == STKVARI8 || a == STKVARI16 || a == STKVARI32 || a == STKVARI64 ||
                      a == GLBVARI8 || a == GLBVARI16 || a == GLBVARI32 || a == GLBVARI64);

    int issigned_b = (b == TMPVARI8 || b == TMPVARI16 || b == TMPVARI32 || b == TMPVARI64 ||
                      b == STKVARI8 || b == STKVARI16 || b == STKVARI32 || b == STKVARI64 ||
                      b == GLBVARI8 || b == GLBVARI16 || b == GLBVARI32 || b == GLBVARI64);

    int max_size = size_a > size_b ? size_a : size_b;

    if (issigned_a != issigned_b && size_a == size_b) {
        switch (max_size) {
            case 1:  return TMPVARU8;
            case 2:  return TMPVARU16;
            case 4:  return TMPVARU32;
            default: return TMPVARU64;
        }
    }

    if (size_a >= size_b) return a;
    else return b;
}

hir_subject_type_t HIR_get_tmp_type(hir_subject_type_t t) {
    switch (t) {
        case TMPVARI8: 
        case STKVARI8: 
        case GLBVARI8: return TMPVARI8;
        
        case TMPVARI16: 
        case STKVARI16: 
        case GLBVARI16: return TMPVARI16;
        
        case TMPVARI32: 
        case STKVARI32: 
        case GLBVARI32: return TMPVARI32;
        
        case TMPVARI64: 
        case STKVARI64: 
        case GLBVARI64: return TMPVARI64;

        case TMPVARU8: 
        case STKVARU8: 
        case GLBVARU8: return TMPVARU8;
        
        case TMPVARU16: 
        case STKVARU16: 
        case GLBVARU16: return TMPVARU16;
        
        case TMPVARU32: 
        case STKVARU32: 
        case GLBVARU32: return TMPVARU32;
        
        case TMPVARU64: 
        case STKVARU64: 
        case GLBVARU64: return TMPVARU64;

        case TMPVARF32: 
        case STKVARF32: 
        case GLBVARF32: return TMPVARF32;
        
        case TMPVARF64: 
        case STKVARF64: 
        case GLBVARF64: return TMPVARF64;

        default: return TMPVARI64;
    }
}

hir_subject_type_t HIR_get_tmptype_tkn(token_t* token) {
    if (!token) return TMPVARI64;
    int bitness  = VRS_variable_bitness(token, 1);
    int isfloat  = VRS_is_float(token);
    int issigned = VRS_issign(token);
    if (!isfloat) {
        switch (bitness) {
            case 8:  return issigned ? TMPVARI8 : TMPVARU8;
            case 16: return issigned ? TMPVARI16 : TMPVARU16;
            case 32: return issigned ? TMPVARI32 : TMPVARU32;
            default: return issigned ? TMPVARI64 : TMPVARU64;
        }
    }

    switch (bitness) {
        case 32: return TMPVARF32;
        default: return TMPVARF64;
    }
}

hir_subject_type_t _get_glbtype(int bitness, int isfloat, int issigned) {
    if (!isfloat) {
        switch (bitness) {
            case 8:  return issigned ? GLBVARI8 : GLBVARU8;
            case 16: return issigned ? GLBVARI16 : GLBVARU16;
            case 32: return issigned ? GLBVARI32 : GLBVARU32;
            default: return issigned ? GLBVARI64 : GLBVARU64;
        }
    }

    switch (bitness) {
        case 32: return GLBVARF32;
        default: return GLBVARF64;
    }
}

hir_subject_type_t HIR_get_stktype(token_t* token) {
    if (!token) return STKVARI64;
    int bitness  = VRS_variable_bitness(token, 1);
    int isfloat  = VRS_is_float(token);
    int issigned = VRS_issign(token);
    int isarr    = token->t_type == ARR_VARIABLE_TOKEN;
    int isstr    = token->t_type == STR_VARIABLE_TOKEN;

    if (!VRS_instack(token)) {
        if (isarr) return GLBVARARR;
        if (isstr) return GLBVARSTR;
        return _get_glbtype(bitness, isfloat, issigned);
    }
    
    if (isarr) return STKVARARR;
    if (isstr) return STKVARSTR;
    if (!isfloat) {
        switch (bitness) {
            case 8:  return issigned ? STKVARI8 : STKVARU8;
            case 16: return issigned ? STKVARI16 : STKVARU16;
            case 32: return issigned ? STKVARI32 : STKVARU32;
            default: return issigned ? STKVARI64 : STKVARU64;
        }
    }

    switch (bitness) {
        case 32: return STKVARF32;
        default: return STKVARF64;
    }
}
