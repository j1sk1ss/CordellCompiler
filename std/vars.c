#include <vars.h>

/* 
ptr - 0 we ignore ptr flag.
Return variable bitness (size in bits). 
*/
int VRS_variable_bitness(token_t* token, char ptr) {
    if (!token) return 8;
    if (ptr && token->vinfo.ptr) return BASE_BITNESS;
    switch (token->t_type) {
        case UNKNOWN_NUMERIC_TOKEN:
        case LONG_TYPE_TOKEN:
        case ULONG_TYPE_TOKEN:
        case LONG_VARIABLE_TOKEN:  
        case ULONG_VARIABLE_TOKEN:  return 64;
        case INT_TYPE_TOKEN:
        case UINT_TYPE_TOKEN:
        case INT_VARIABLE_TOKEN:   
        case UINT_VARIABLE_TOKEN:   return 32;
        case SHORT_TYPE_TOKEN:
        case USHORT_TYPE_TOKEN:
        case SHORT_VARIABLE_TOKEN: 
        case USHORT_VARIABLE_TOKEN: return 16;
        case CHAR_TYPE_TOKEN:
        case UCHAR_TYPE_TOKEN:
        case CHAR_VALUE_TOKEN:
        case CHAR_VARIABLE_TOKEN:  
        case UCHAR_VARIABLE_TOKEN:  return 8;
        default:                    return 8;
    }

    return 8;
} 

/* Return 1 if token is pointer (arr, string, ptr). Otherwise return 0. */
int VRS_isptr(token_t* token) {
    if (!token) return 0;
    if (token->vinfo.ptr) return 1;
    switch (token->t_type) {
        case UNKNOWN_NUMERIC_TOKEN:
        case LONG_VARIABLE_TOKEN:
        case INT_VARIABLE_TOKEN:
        case SHORT_VARIABLE_TOKEN:
        case ULONG_VARIABLE_TOKEN:
        case UINT_VARIABLE_TOKEN:
        case USHORT_VARIABLE_TOKEN:
        case UCHAR_VARIABLE_TOKEN:
        case CHAR_VALUE_TOKEN:
        case CHAR_VARIABLE_TOKEN:  return 0;
        case STRING_VALUE_TOKEN:
        case ARR_VARIABLE_TOKEN:
        case STR_VARIABLE_TOKEN:   return 1;
        default:                   return 0;
    }

    return 0;
}

/* Is token in text segment */
int VRS_intext(token_t* token) {
    if (!token) return 0;
    return !token->vinfo.glob && !token->vinfo.ro;
}

/* Is variable occupie one slot in stack? */
int VRS_one_slot(token_t* token) {
    if (!token) return 0;
    if (token->vinfo.ptr) return 1;
    switch (token->t_type) {
        case INT_TYPE_TOKEN:
        case LONG_TYPE_TOKEN:
        case CHAR_TYPE_TOKEN:
        case SHORT_TYPE_TOKEN:
        case UINT_TYPE_TOKEN:
        case ULONG_TYPE_TOKEN:
        case UCHAR_TYPE_TOKEN:
        case USHORT_TYPE_TOKEN:
        case CHAR_VALUE_TOKEN:
        case INT_VARIABLE_TOKEN:
        case LONG_VARIABLE_TOKEN:
        case CHAR_VARIABLE_TOKEN:
        case SHORT_VARIABLE_TOKEN:
        case UINT_VARIABLE_TOKEN:
        case ULONG_VARIABLE_TOKEN:
        case UCHAR_VARIABLE_TOKEN:
        case USHORT_VARIABLE_TOKEN:
        case UNKNOWN_NUMERIC_TOKEN: return 1;
        case STR_TYPE_TOKEN:
        case ARRAY_TYPE_TOKEN:
        case STRING_VALUE_TOKEN:
        case ARR_VARIABLE_TOKEN:
        case STR_VARIABLE_TOKEN:
        default:                    return 0;
    }

    return 0;
}

/* Is close token? */
int VRS_isclose(token_t* token) {
    if (!token) return 1;
    switch (token->t_type) {
        case COMMA_TOKEN:
        case DELIMITER_TOKEN:
        case CLOSE_INDEX_TOKEN:
        case CLOSE_BLOCK_TOKEN:
        case CLOSE_BRACKET_TOKEN: return 1;
        default: return 0;
    }
}

int VRS_isdecl(token_t* token) {
    if (!token) return 0;
    switch (token->t_type) {
        case INT_TYPE_TOKEN:
        case STR_TYPE_TOKEN:
        case CHAR_TYPE_TOKEN:
        case LONG_TYPE_TOKEN:
        case ARRAY_TYPE_TOKEN:
        case SHORT_TYPE_TOKEN:
        case UINT_TYPE_TOKEN:
        case UCHAR_TYPE_TOKEN:
        case ULONG_TYPE_TOKEN:
        case USHORT_TYPE_TOKEN:    return 1;
        default:                   return 0;
    }
}

int VRS_isblock(token_t* token) {
    return !token || (token->t_type == SCOPE_TOKEN) || (token->t_type == START_TOKEN);
}

int VRS_isoperand(token_t* token) {
    if (!token) return 0;
    switch (token->t_type) {
        case PLUS_TOKEN:
        case MINUS_TOKEN:
        case BITOR_TOKEN:
        case LOWER_TOKEN:
        case BITXOR_TOKEN:
        case ASSIGN_TOKEN:
        case LARGER_TOKEN:
        case DIVIDE_TOKEN:
        case BITAND_TOKEN:
        case COMPARE_TOKEN:
        case NCOMPARE_TOKEN:
        case MULTIPLY_TOKEN:
        case BITMOVE_LEFT_TOKEN:
        case BITMOVE_RIGHT_TOKEN:  return 1;
        default:                   return 0;
    }
}

int VRS_token_priority(token_t* token) {
    if (!token) return -1;
    switch (token->t_type) {
        case OR_TOKEN:             return 1;
        case AND_TOKEN:            return 2;
        case BITOR_TOKEN:          return 3;
        case BITXOR_TOKEN:         return 4;
        case BITAND_TOKEN:         return 5;
        case COMPARE_TOKEN:
        case NCOMPARE_TOKEN:
        case LOWER_TOKEN:
        case LARGER_TOKEN:         return 6;
        case BITMOVE_LEFT_TOKEN:
        case BITMOVE_RIGHT_TOKEN:  return 7;
        case PLUS_TOKEN:
        case MINUS_TOKEN:          return 8;
        case MULTIPLY_TOKEN:
        case DIVIDE_TOKEN:
        case MODULO_TOKEN:         return 9;
        case ASSIGN_TOKEN:         return 0;
        default:                   return -1;
    }
}

int VRS_isnumeric(token_t* token) {
    if (!token) return 0;
    if (
        token->t_type == UNKNOWN_NUMERIC_TOKEN || 
        token->t_type == UNKNOWN_CHAR_TOKEN
    ) return 1;
    return 0;
}

int VRS_isvariable(token_t* token) {
    if (!token) return 0;
    switch (token->t_type) {
        case ARR_VARIABLE_TOKEN:
        case STR_VARIABLE_TOKEN:
        case LONG_VARIABLE_TOKEN:
        case INT_VARIABLE_TOKEN:
        case SHORT_VARIABLE_TOKEN:
        case CHAR_VARIABLE_TOKEN:
        case ULONG_VARIABLE_TOKEN:
        case UINT_VARIABLE_TOKEN:
        case USHORT_VARIABLE_TOKEN:
        case UCHAR_VARIABLE_TOKEN: return 1;
        default: return 0;
    }
}

int VRS_instant_movable(token_t* token) {
    return VRS_isnumeric(token) || (VRS_isvariable(token) && VRS_one_slot(token));
}

int VRS_issign(token_t* token) {
    if (!token) return 0;
    switch (token->t_type) {
        case LONG_VARIABLE_TOKEN:
        case INT_VARIABLE_TOKEN:
        case SHORT_VARIABLE_TOKEN:
        case CHAR_VARIABLE_TOKEN:
        case LONG_TYPE_TOKEN:
        case INT_TYPE_TOKEN:
        case SHORT_TYPE_TOKEN:
        case CHAR_TYPE_TOKEN:      return 1;
        case ULONG_VARIABLE_TOKEN:
        case UINT_VARIABLE_TOKEN:
        case USHORT_VARIABLE_TOKEN:
        case UCHAR_VARIABLE_TOKEN:
        case ULONG_TYPE_TOKEN:
        case UINT_TYPE_TOKEN:
        case USHORT_TYPE_TOKEN:
        case UCHAR_TYPE_TOKEN:     return 0;
        default: return 1;
    }
}
