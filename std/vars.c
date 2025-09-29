#include <std/vars.h>

/* 
ptr - 0 we ignore ptr flag.
Return variable bitness (size in bits). 
*/
int VRS_variable_bitness(token_t* token, char ptr) {
    if (!token) return 8;
    if (ptr && token->flags.ptr) return BASE_BITNESS;
    switch (token->t_type) {
        case UNKNOWN_NUMERIC_TOKEN:
        case I64_TYPE_TOKEN:
        case U64_TYPE_TOKEN:
        case F64_TYPE_TOKEN:
        case I64_VARIABLE_TOKEN:  
        case U64_VARIABLE_TOKEN: 
        case F64_VARIABLE_TOKEN: return BASE_BITNESS;
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

    return BASE_BITNESS;
} 

/* Return 1 if token is pointer (arr, string, ptr). Otherwise return 0. */
int VRS_isptr(token_t* token) {
    if (!token) return 0;
    if (token->flags.ptr) return 1;
    switch (token->t_type) {
        case UNKNOWN_NUMERIC_TOKEN:
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
        case STRING_VALUE_TOKEN:
        case ARR_VARIABLE_TOKEN:
        case STR_VARIABLE_TOKEN:   return 1;
        default:                   return 0;
    }

    return 0;
}

/* Is token in text segment */
int VRS_instack(token_t* token) {
    if (!token) return 0;
    return !token->flags.glob && !token->flags.ro && !token->flags.ext;
}

/* Is variable occupie one slot in stack? */
int VRS_one_slot(token_t* token) {
    if (!token) return 0;
    if (token->flags.ptr) return 1;
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
        case ARRAY_TYPE_TOKEN:  return 1;
        default:                return 0;
    }
}

int VRS_isblock(token_t* token) {
    return (
        !token || 
        (token->t_type == SCOPE_TOKEN) || 
        (token->t_type == START_TOKEN)
    );
}

int VRS_isoperand(token_t* token) {
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
        case COMPARE_TOKEN:
        case LOWEREQ_TOKEN:
        case LARGEREQ_TOKEN:
        case NCOMPARE_TOKEN:
        case MULTIPLY_TOKEN:
        case ADDASSIGN_TOKEN:
        case SUBASSIGN_TOKEN:
        case MULASSIGN_TOKEN:
        case DIVASSIGN_TOKEN:
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
        case ADDASSIGN_TOKEN:
        case SUBASSIGN_TOKEN:
        case MULASSIGN_TOKEN:
        case DIVASSIGN_TOKEN:
        case ASSIGN_TOKEN:         return 0;
        default:                   return -1;
    }
}

int VRS_isnumeric(token_t* token) {
    if (!token) return 0;
    if (
        token->t_type == UNKNOWN_NUMERIC_TOKEN || 
        token->t_type == UNKNOWN_CHAR_TOKEN ||
        token->t_type == CHAR_VALUE_TOKEN
    ) return 1;
    return 0;
}

int VRS_isvariable(token_t* token) {
    if (!token) return 0;
    switch (token->t_type) {
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

int VRS_instant_movable(token_t* token) {
    return VRS_isnumeric(token) || (VRS_isvariable(token) && VRS_one_slot(token));
}

int VRS_issign(token_t* token) {
    if (!token) return 0;
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
        case F32_TYPE_TOKEN:     return 0;

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

int VRS_is_control_change(token_t* token) {
    if (!token) return 0;
    switch (token->t_type) {
        case IF_TOKEN:
        case CALL_TOKEN:
        case EXIT_TOKEN:
        case WHILE_TOKEN:
        case SWITCH_TOKEN: return 1;
        default:           return 0;
    }
}

int VRS_is_unpredicted(token_t* token) {
    if (!token) return 0;
    switch (token->t_type) {
        case IF_TOKEN:
        case WHILE_TOKEN:
        case SWITCH_TOKEN: return 1;
        default:           return 0;
    }
}

int VRS_is_float(token_t* token) {
    if (!token) return 0;
    switch (token->t_type) {
        case F64_VARIABLE_TOKEN:
        case F32_VARIABLE_TOKEN:
        case F64_TYPE_TOKEN:
        case F32_TYPE_TOKEN:    return 1;
        default:                return 0;
    }
}

int VRS_update_operator(token_t* token) {
    if (!token) return 0;
    switch (token->t_type) {
        case ASSIGN_TOKEN:
        case ADDASSIGN_TOKEN:
        case SUBASSIGN_TOKEN:
        case DIVASSIGN_TOKEN:
        case MULASSIGN_TOKEN:   return 1;
        default:                return 0;
    }
}

