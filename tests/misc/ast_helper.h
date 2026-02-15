#ifndef ASTHELPER_H_
#define ASTHELPER_H_
#include <ast/astgen.h>
const char* name_tkn_type(token_type_t t) {
    switch (t) {
        case INDEXATION_TOKEN:            return "INDEXATION_TOKEN";
        case UNKNOWN_FLOAT_NUMERIC_TOKEN: return "UNKNOWN_FLOAT_NUMERIC_TOKEN";
        case UNKNOWN_CHAR_TOKEN:          return "UNKNOWN_CHAR_TOKEN";
        case UNKNOWN_BRACKET_VALUE:       return "UNKNOWN_BRACKET_VALUE";
        case UNKNOWN_STRING_TOKEN:        return "UNKNOWN_STRING_TOKEN";
        case UNKNOWN_NUMERIC_TOKEN:       return "UNKNOWN_NUMERIC_TOKEN";
        case DREF_TYPE_TOKEN:             return "DREF_TYPE_TOKEN";
        case REF_TYPE_TOKEN:              return "REF_TYPE_TOKEN";
        case NEGATIVE_TOKEN:              return "NEGATIVE_TOKEN";
        case VAR_ARGUMENTS_TOKEN:         return "VAR_ARGUMENTS_TOKEN";
        case I0_TYPE_TOKEN:               return "I0_TYPE_TOKEN";
        case F64_TYPE_TOKEN:              return "F64_TYPE_TOKEN";
        case F32_TYPE_TOKEN:              return "F32_TYPE_TOKEN";
        case I64_TYPE_TOKEN:              return "I64_TYPE_TOKEN";
        case I32_TYPE_TOKEN:              return "I32_TYPE_TOKEN";
        case I16_TYPE_TOKEN:              return "I16_TYPE_TOKEN";
        case I8_TYPE_TOKEN:               return "I8_TYPE_TOKEN";
        case U64_TYPE_TOKEN:              return "U64_TYPE_TOKEN";
        case U32_TYPE_TOKEN:              return "U32_TYPE_TOKEN";
        case U16_TYPE_TOKEN:              return "U16_TYPE_TOKEN";
        case U8_TYPE_TOKEN:               return "U8_TYPE_TOKEN";
        case STR_TYPE_TOKEN:              return "STR_TYPE_TOKEN";
        case ARRAY_TYPE_TOKEN:            return "ARRAY_TYPE_TOKEN";
        case CONVERT_TOKEN:               return "CONVERT_TOKEN";
        case IMPORT_TOKEN:                return "IMPORT_TOKEN";
        case IMPORT_SELECT_TOKEN:         return "IMPORT_SELECT_TOKEN";
        case EXTERN_TOKEN:                return "EXTERN_TOKEN";
        case START_TOKEN:                 return "START_TOKEN";
        case RETURN_TOKEN:                return "RETURN_TOKEN";
        case EXIT_TOKEN:                  return "EXIT_TOKEN";
        case RETURN_TYPE_TOKEN:           return "RETURN_TYPE_TOKEN";
        case SCOPE_TOKEN:                 return "SCOPE_TOKEN";
        case ASM_TOKEN:                   return "ASM_TOKEN";
        case SYSCALL_TOKEN:               return "SYSCALL_TOKEN";
        case EXFUNC_TOKEN:                return "EXFUNC_TOKEN";
        case FUNC_PROT_TOKEN:             return "FUNC_PROT_TOKEN";
        case FUNC_TOKEN:                  return "FUNC_TOKEN";
        case FUNC_NAME_TOKEN:             return "FUNC_NAME_TOKEN";
        case CALL_TOKEN:                  return "CALL_TOKEN";
        case SWITCH_TOKEN:                return "SWITCH_TOKEN";
        case CASE_TOKEN:                  return "CASE_TOKEN";
        case DEFAULT_TOKEN:               return "DEFAULT_TOKEN";
        case WHILE_TOKEN:                 return "WHILE_TOKEN";
        case LOOP_TOKEN:                  return "LOOP_TOKEN";
        case BREAK_TOKEN:                 return "BREAK_TOKEN";
        case IF_TOKEN:                    return "IF_TOKEN";
        case ELSE_TOKEN:                  return "ELSE_TOKEN";
        case PLUS_TOKEN:                  return "PLUS_TOKEN";
        case MINUS_TOKEN:                 return "MINUS_TOKEN";
        case MULTIPLY_TOKEN:              return "MULTIPLY_TOKEN";
        case DIVIDE_TOKEN:                return "DIVIDE_TOKEN";
        case MODULO_TOKEN:                return "MODULO_TOKEN";
        case ADDASSIGN_TOKEN:             return "ADDASSIGN_TOKEN";
        case SUBASSIGN_TOKEN:             return "SUBASSIGN_TOKEN";
        case MULASSIGN_TOKEN:             return "MULASSIGN_TOKEN";
        case DIVASSIGN_TOKEN:             return "DIVASSIGN_TOKEN";
        case MODULOASSIGN_TOKEN:          return "MODULOASSIGN_TOKEN";
        case BITANDASSIGN_TOKEN:          return "BITANDASSIGN_TOKEN";
        case BITORASSIGN_TOKEN:           return "BITORASSIGN_TOKEN";
        case BITXORASSIGN_TOKEN:          return "BITXORASSIGN_TOKEN";
        case ASSIGN_TOKEN:                return "ASSIGN_TOKEN";
        case COMPARE_TOKEN:               return "COMPARE_TOKEN";
        case NCOMPARE_TOKEN:              return "NCOMPARE_TOKEN";
        case LOWER_TOKEN:                 return "LOWER_TOKEN";
        case LOWEREQ_TOKEN:               return "LOWEREQ_TOKEN";
        case LARGER_TOKEN:                return "LARGER_TOKEN";
        case LARGEREQ_TOKEN:              return "LARGEREQ_TOKEN";
        case BITMOVE_LEFT_TOKEN:          return "BITMOVE_LEFT_TOKEN";
        case BITMOVE_RIGHT_TOKEN:         return "BITMOVE_RIGHT_TOKEN";
        case BITAND_TOKEN:                return "BITAND_TOKEN";
        case BITOR_TOKEN:                 return "BITOR_TOKEN";
        case BITXOR_TOKEN:                return "BITXOR_TOKEN";
        case AND_TOKEN:                   return "AND_TOKEN";
        case OR_TOKEN:                    return "OR_TOKEN";
        case VARIABLE_TOKEN:              return "VARIABLE_TOKEN";
        case F64_VARIABLE_TOKEN:          return "F64_VARIABLE_TOKEN";
        case F32_VARIABLE_TOKEN:          return "F32_VARIABLE_TOKEN";
        case I64_VARIABLE_TOKEN:          return "I64_VARIABLE_TOKEN";
        case I32_VARIABLE_TOKEN:          return "I32_VARIABLE_TOKEN";
        case I16_VARIABLE_TOKEN:          return "I16_VARIABLE_TOKEN";
        case I8_VARIABLE_TOKEN:           return "I8_VARIABLE_TOKEN";
        case U64_VARIABLE_TOKEN:          return "U64_VARIABLE_TOKEN";
        case U32_VARIABLE_TOKEN:          return "U32_VARIABLE_TOKEN";
        case U16_VARIABLE_TOKEN:          return "U16_VARIABLE_TOKEN";
        case U8_VARIABLE_TOKEN:           return "U8_VARIABLE_TOKEN";
        case STR_VARIABLE_TOKEN:          return "STR_VARIABLE_TOKEN";
        case ARR_VARIABLE_TOKEN:          return "ARR_VARIABLE_TOKEN";
        case STRING_VALUE_TOKEN:          return "STRING_VALUE_TOKEN";
        case CHAR_VALUE_TOKEN:            return "CHAR_VALUE_TOKEN";
        case BREAKPOINT_TOKEN:            return "BREAKPOINT_TOKEN";
        default: return "";
    }
}

const char* fmt_tkn_type(token_t* t) {
    if (!t) return "";
    if (t->flags.ptr) {
        switch (t->t_type) {
            case I0_TYPE_TOKEN:  return "i0*";
            case I8_TYPE_TOKEN:  return "i8*";
            case U8_TYPE_TOKEN:  return "u8*";
            case I16_TYPE_TOKEN: return "i16*";
            case U16_TYPE_TOKEN: return "u16*";
            case I32_TYPE_TOKEN: return "i32*";
            case U32_TYPE_TOKEN: return "u32*";
            case F32_TYPE_TOKEN: return "f32*";
            case I64_TYPE_TOKEN: return "i64*";
            case U64_TYPE_TOKEN: return "u64*";
            case F64_TYPE_TOKEN: return "f64*";
            case STR_TYPE_TOKEN: return "str*";
            default: return "";
        }
    }
    switch (t->t_type) {
        case I0_TYPE_TOKEN:  return "i0";
        case I8_TYPE_TOKEN:  return "i8";
        case U8_TYPE_TOKEN:  return "u8";
        case I16_TYPE_TOKEN: return "i16";
        case U16_TYPE_TOKEN: return "u16";
        case I32_TYPE_TOKEN: return "i32";
        case U32_TYPE_TOKEN: return "u32";
        case F32_TYPE_TOKEN: return "f32";
        case I64_TYPE_TOKEN: return "i64";
        case U64_TYPE_TOKEN: return "u64";
        case F64_TYPE_TOKEN: return "f64";
        case STR_TYPE_TOKEN: return "str";
        default: return "";
    }
}

static inline int print_ast(ast_node_t* node, int depth) {
    if (!node) return 0;
    for (int i = 0; i < depth; i++) printf("   ");
    if (node->t) {
        switch (node->t->t_type) {
            case SCOPE_TOKEN: printf("{ scope, id=%i }\n", node->sinfo.s_id); break;
            case INDEXATION_TOKEN: printf("[[]]\n");                          break;
            default:
                printf(
                    "[%s] (%s,%sv_id=%i, s_id=%i%s%s%s%s)\n",
                    node->t->body->body, name_tkn_type(node->t->t_type), 
                    node->t->flags.ptr ? " ptr, " : " ",
                    node->sinfo.v_id, node->sinfo.s_id,
                    node->t->flags.ro ? ", ro" : "",
                    node->t->flags.ext ? ", ext" : "",
                    node->t->flags.glob ? ", glob" : "",
                    node->t->flags.heap ? ", heap" : ""
                );
            break;
        }
    }
    else {
        printf("[ block ]\n");
    }
    
    ast_node_t* child = node->c;
    while (child) {
        print_ast(child, depth + 1);
        child = child->siblings.n;
    }
    
    return 1;
}
#endif