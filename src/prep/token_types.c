#include <prep/token_types.h>

/* ptr - 0 we ignore ptr flag.
Return variable bitness (size in bits). 
*/
int TKN_variable_bitness(token_t* token, char ptr) {
    if (!token) return 8;
    if (ptr && token->flags.ptr) return 64;
    switch (token->t_type) {
        case UNKNOWN_NUMERIC_TOKEN:
        case UNKNOWN_FLOAT_NUMERIC_TOKEN:
        case I64_TYPE_TOKEN:
        case U64_TYPE_TOKEN:
        case F64_TYPE_TOKEN:
        case I64_VARIABLE_TOKEN:  
        case U64_VARIABLE_TOKEN: 
        case F64_VARIABLE_TOKEN: return 64;
        case I32_TYPE_TOKEN:
        case U32_TYPE_TOKEN:
        case F32_TYPE_TOKEN:
        case I32_VARIABLE_TOKEN:   
        case U32_VARIABLE_TOKEN: 
        case F32_VARIABLE_TOKEN: return 32;
        case I16_TYPE_TOKEN:
        case U16_TYPE_TOKEN:
        case I16_VARIABLE_TOKEN: 
        case U16_VARIABLE_TOKEN: return 16;
        case I8_TYPE_TOKEN:
        case U8_TYPE_TOKEN:
        case CHAR_VALUE_TOKEN:
        case I8_VARIABLE_TOKEN:  
        case U8_VARIABLE_TOKEN:  return 8;
        default:                 return 8;
    }

    return 64;
} 

token_type_t TKN_get_tmp_type(token_type_t t) {
    switch (t) {
        case F64_TYPE_TOKEN: return TMP_F64_TYPE_TOKEN;
        case F32_TYPE_TOKEN: return TMP_F32_TYPE_TOKEN;
        case I64_TYPE_TOKEN: return TMP_I64_TYPE_TOKEN;
        case I32_TYPE_TOKEN: return TMP_I32_TYPE_TOKEN;
        case I16_TYPE_TOKEN: return TMP_I16_TYPE_TOKEN;
        case I8_TYPE_TOKEN:  return TMP_I8_TYPE_TOKEN;
        case U64_TYPE_TOKEN: return TMP_U64_TYPE_TOKEN;
        case U32_TYPE_TOKEN: return TMP_U32_TYPE_TOKEN;
        case U16_TYPE_TOKEN: return TMP_U16_TYPE_TOKEN;
        case U8_TYPE_TOKEN:  return TMP_U8_TYPE_TOKEN;
        default: return TMP_I64_TYPE_TOKEN;
    }
}

int TKN_istmp_type(token_type_t t) {
    switch (t) {
        case TMP_F64_TYPE_TOKEN:
        case TMP_F32_TYPE_TOKEN:
        case TMP_I64_TYPE_TOKEN:
        case TMP_I32_TYPE_TOKEN:
        case TMP_I16_TYPE_TOKEN:
        case TMP_I8_TYPE_TOKEN:
        case TMP_U64_TYPE_TOKEN:
        case TMP_U32_TYPE_TOKEN:
        case TMP_U16_TYPE_TOKEN:
        case TMP_U8_TYPE_TOKEN:  return 1;
        default: return 0; 
    }
}

/* Return 1 if token is pointer (arr, string, ptr). Otherwise return 0. */
int TKN_isptr(token_t* token) {
    if (!token) return 0;
    if (token->flags.ptr) return 1;
    switch (token->t_type) {
        case UNKNOWN_NUMERIC_TOKEN:
        case UNKNOWN_FLOAT_NUMERIC_TOKEN:
        case I64_VARIABLE_TOKEN:
        case I32_VARIABLE_TOKEN:
        case I16_VARIABLE_TOKEN:
        case I8_VARIABLE_TOKEN:
        case U64_VARIABLE_TOKEN:
        case U32_VARIABLE_TOKEN:
        case U16_VARIABLE_TOKEN:
        case U8_VARIABLE_TOKEN:
        case F64_VARIABLE_TOKEN:
        case F32_VARIABLE_TOKEN:
        case CHAR_VALUE_TOKEN:     return 0;
        case ARR_VARIABLE_TOKEN:
        case STR_VARIABLE_TOKEN:   return 1;
        default:                   return 0;
    }

    return 0;
}

/* Is token in text segment */
int TKN_instack(token_t* token) {
    if (!token) return 0;
    return !token->flags.glob && !token->flags.ro && !token->flags.ext;
}

/* Is variable occupie one slot in stack? */
int TKN_one_slot(token_t* token) {
    if (!token) return 0;
    if (
        token->flags.ptr > 0 &&                     /* If this is a pointer                          */
        (
            token->flags.ptr > 0 ||                 /* And if this a pointer to an array or a string */
            (                                       /* It must be a pointer to a pointer             */
                token->t_type != STR_TYPE_TOKEN     && 
                token->t_type != STR_VARIABLE_TOKEN &&
                token->t_type != ARRAY_TYPE_TOKEN   &&
                token->t_type != ARR_VARIABLE_TOKEN
            )
        )
    ) return 1;
    
    switch (token->t_type) {
        case I8_TYPE_TOKEN:
        case U8_TYPE_TOKEN:
        case I16_TYPE_TOKEN:
        case U16_TYPE_TOKEN:
        case I32_TYPE_TOKEN:
        case U32_TYPE_TOKEN:
        case F32_TYPE_TOKEN:
        case I64_TYPE_TOKEN:
        case U64_TYPE_TOKEN:
        case F64_TYPE_TOKEN:
        case CHAR_VALUE_TOKEN:
        case I8_VARIABLE_TOKEN:
        case U8_VARIABLE_TOKEN:
        case I16_VARIABLE_TOKEN:
        case U16_VARIABLE_TOKEN:
        case I32_VARIABLE_TOKEN:
        case U32_VARIABLE_TOKEN:
        case F32_VARIABLE_TOKEN:
        case I64_VARIABLE_TOKEN:
        case U64_VARIABLE_TOKEN:
        case F64_VARIABLE_TOKEN:
        case UNKNOWN_NUMERIC_TOKEN:
        case UNKNOWN_FLOAT_NUMERIC_TOKEN: return 1;
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
int TKN_isclose(token_t* token) {
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

int TKN_isdecl(token_t* token) {
    if (!token) return 0;
    switch (token->t_type) {
        case I8_TYPE_TOKEN:
        case U8_TYPE_TOKEN:
        case I16_TYPE_TOKEN:
        case U16_TYPE_TOKEN:
        case I32_TYPE_TOKEN:
        case U32_TYPE_TOKEN:
        case F32_TYPE_TOKEN:
        case I64_TYPE_TOKEN:
        case U64_TYPE_TOKEN:
        case F64_TYPE_TOKEN:
        case STR_TYPE_TOKEN:
        case ARRAY_TYPE_TOKEN: return 1;
        default:               return 0;
    }
}

int TKN_isblock(token_t* token) {
    return (
        !token || 
        (token->t_type == SCOPE_TOKEN) || 
        (token->t_type == START_TOKEN)
    );
}

int TKN_isoperand(token_t* token) {
    if (!token) return 0;
    switch (token->t_type) {
        case OR_TOKEN:
        case AND_TOKEN:
        case PLUS_TOKEN:
        case MINUS_TOKEN:
        case BITOR_TOKEN:
        case LOWER_TOKEN:
        case BITXOR_TOKEN:
        case ASSIGN_TOKEN:
        case LARGER_TOKEN:
        case DIVIDE_TOKEN:
        case BITAND_TOKEN:
        case MODULO_TOKEN:
        case COMPARE_TOKEN:
        case LOWEREQ_TOKEN:
        case LARGEREQ_TOKEN:
        case NCOMPARE_TOKEN:
        case MULTIPLY_TOKEN:
        case ADDASSIGN_TOKEN:
        case SUBASSIGN_TOKEN:
        case MULASSIGN_TOKEN:
        case DIVASSIGN_TOKEN:
        case BITORASSIGN_TOKEN:
        case MODULOASSIGN_TOKEN:
        case BITANDASSIGN_TOKEN:
        case BITXORASSIGN_TOKEN:
        case BITMOVE_LEFT_TOKEN:
        case BITMOVE_RIGHT_TOKEN: return 1;
        default:                  return 0;
    }
}

int TKN_token_priority(token_t* token) {
    if (!token) return -1;
    switch (token->t_type) {
        case OR_TOKEN:             return 1;
        case AND_TOKEN:            return 2;
        case BITOR_TOKEN:          return 3;
        case BITXOR_TOKEN:         return 4;
        case BITAND_TOKEN:         return 5;
        case COMPARE_TOKEN:
        case NCOMPARE_TOKEN:
        case LOWEREQ_TOKEN:
        case LOWER_TOKEN:
        case LARGEREQ_TOKEN:
        case LARGER_TOKEN:         return 6;
        case BITMOVE_LEFT_TOKEN:
        case BITMOVE_RIGHT_TOKEN:  return 7;
        case PLUS_TOKEN:
        case MINUS_TOKEN:          return 8;
        case MULTIPLY_TOKEN:
        case DIVIDE_TOKEN:
        case MODULO_TOKEN:         return 9;
        case BITORASSIGN_TOKEN:
        case MODULOASSIGN_TOKEN:
        case BITANDASSIGN_TOKEN:
        case BITXORASSIGN_TOKEN:
        case ADDASSIGN_TOKEN:
        case SUBASSIGN_TOKEN:
        case MULASSIGN_TOKEN:
        case DIVASSIGN_TOKEN:
        case ASSIGN_TOKEN:         return 0;
        default:                   return -1;
    }
}

int TKN_isnumeric(token_t* token) {
    if (!token) return 0;
    if (
        token->t_type == UNKNOWN_FLOAT_NUMERIC_TOKEN ||
        token->t_type == UNKNOWN_NUMERIC_TOKEN       || 
        token->t_type == UNKNOWN_CHAR_TOKEN          ||
        token->t_type == CHAR_VALUE_TOKEN
    ) return 1;
    return 0;
}

int TKN_isvariable(token_t* token) {
    if (!token) return 0;
    switch (token->t_type) {
        case VARIABLE_TOKEN:
        case ARR_VARIABLE_TOKEN:
        case STR_VARIABLE_TOKEN:
        case F64_VARIABLE_TOKEN:
        case F32_VARIABLE_TOKEN:
        case I64_VARIABLE_TOKEN:
        case I32_VARIABLE_TOKEN:
        case I16_VARIABLE_TOKEN:
        case I8_VARIABLE_TOKEN:
        case U64_VARIABLE_TOKEN:
        case U32_VARIABLE_TOKEN:
        case U16_VARIABLE_TOKEN:
        case U8_VARIABLE_TOKEN: return 1;
        default: return 0;
    }
}

int TKN_issign(token_t* token) {
    if (!token) return 0;
    if (token->flags.ptr) return 0;
    switch (token->t_type) {
        case I64_VARIABLE_TOKEN:
        case I32_VARIABLE_TOKEN:
        case I16_VARIABLE_TOKEN:
        case I8_VARIABLE_TOKEN:
        case I64_TYPE_TOKEN:
        case I32_TYPE_TOKEN:
        case I16_TYPE_TOKEN:
        case I8_TYPE_TOKEN:      return 1;

        case F64_VARIABLE_TOKEN:
        case F32_VARIABLE_TOKEN:
        case F64_TYPE_TOKEN:
        case F32_TYPE_TOKEN:
        case U64_VARIABLE_TOKEN:
        case U32_VARIABLE_TOKEN:
        case U16_VARIABLE_TOKEN:
        case U8_VARIABLE_TOKEN:
        case U64_TYPE_TOKEN:
        case U32_TYPE_TOKEN:
        case U16_TYPE_TOKEN:
        case U8_TYPE_TOKEN:      return 0;
        default: return 1;
    }
}

int TKN_is_float(token_t* token) {
    if (!token) return 0;
    switch (token->t_type) {
        case F64_VARIABLE_TOKEN:
        case F32_VARIABLE_TOKEN:
        case F64_TYPE_TOKEN:
        case F32_TYPE_TOKEN:    return 1;
        default:                return 0;
    }
}

int TKN_update_operator(token_t* token) {
    if (!token) return 0;
    switch (token->t_type) {
        case ASSIGN_TOKEN:
        case BITORASSIGN_TOKEN:
        case MODULOASSIGN_TOKEN:
        case BITANDASSIGN_TOKEN:
        case BITXORASSIGN_TOKEN:
        case ADDASSIGN_TOKEN:
        case SUBASSIGN_TOKEN:
        case DIVASSIGN_TOKEN:
        case MULASSIGN_TOKEN: return 1;
        default:              return 0;
    }
}

token_type_t TKN_get_var_from_type(token_type_t t) {
    switch (t) {
        case STR_TYPE_TOKEN:   return STR_VARIABLE_TOKEN;
        case ARRAY_TYPE_TOKEN: return ARR_VARIABLE_TOKEN;
        case F64_TYPE_TOKEN:   return F64_VARIABLE_TOKEN;
        case F32_TYPE_TOKEN:   return F32_VARIABLE_TOKEN;
        case U64_TYPE_TOKEN:   return U64_VARIABLE_TOKEN;
        case U32_TYPE_TOKEN:   return U32_VARIABLE_TOKEN;
        case U16_TYPE_TOKEN:   return U16_VARIABLE_TOKEN;
        case U8_TYPE_TOKEN:    return U8_VARIABLE_TOKEN;
        case I64_TYPE_TOKEN:   return I64_VARIABLE_TOKEN;
        case I32_TYPE_TOKEN:   return I32_VARIABLE_TOKEN;
        case I16_TYPE_TOKEN:   return I16_VARIABLE_TOKEN;
        case I8_TYPE_TOKEN:    return I8_VARIABLE_TOKEN;
        default:               return U64_VARIABLE_TOKEN;
    }
}
