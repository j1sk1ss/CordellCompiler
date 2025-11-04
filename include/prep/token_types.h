#ifndef TOKEN_TYPES_H_
#define TOKEN_TYPES_H_

typedef enum {
    // Unknowns
    UNKNOWN_CHAR_TOKEN,
    UNKNOWN_BRACKET_VALUE,
    UNKNOWN_STRING_TOKEN,
    UNKNOWN_NUMERIC_TOKEN,
    LINE_BREAK_TOKEN,

    COMMENT_TOKEN,
    DELIMITER_TOKEN,
    COMMA_TOKEN,
    OPEN_INDEX_TOKEN,
    CLOSE_INDEX_TOKEN,
    OPEN_BRACKET_TOKEN,
    CLOSE_BRACKET_TOKEN,
    OPEN_BLOCK_TOKEN,
    CLOSE_BLOCK_TOKEN,

    // Modifiers
    DREF_TYPE_TOKEN,     // dref
    REF_TYPE_TOKEN,      // ref
    PTR_TYPE_TOKEN,      // ptr
    RO_TYPE_TOKEN,       // ro
    GLOB_TYPE_TOKEN,     // glob
    NEGATIVE_TOKEN,      // not

    // Types
    TMP_F64_TYPE_TOKEN,  // tmp_f64
    TMP_F32_TYPE_TOKEN,  // tmp_f32
    TMP_I64_TYPE_TOKEN,  // tmp_i64
    TMP_I32_TYPE_TOKEN,  // tmp_i32
    TMP_I16_TYPE_TOKEN,  // tmp_i16
    TMP_I8_TYPE_TOKEN,   // tmp_i8
    TMP_U64_TYPE_TOKEN,  // tmp_u64
    TMP_U32_TYPE_TOKEN,  // tmp_u32
    TMP_U16_TYPE_TOKEN,  // tmp_u16
    TMP_U8_TYPE_TOKEN,   // tmp_u8

    F64_TYPE_TOKEN,      // f64
    F32_TYPE_TOKEN,      // f32
    I64_TYPE_TOKEN,      // i64
    I32_TYPE_TOKEN,      // i32
    I16_TYPE_TOKEN,      // i16
    I8_TYPE_TOKEN,       // i8
    U64_TYPE_TOKEN,      // u64
    U32_TYPE_TOKEN,      // u32
    U16_TYPE_TOKEN,      // u16
    U8_TYPE_TOKEN,       // u8
    STR_TYPE_TOKEN,      // str
    ARRAY_TYPE_TOKEN,    // arr
    
    // Commands
    IMPORT_TOKEN,        // import
    IMPORT_SELECT_TOKEN, // from
    EXTERN_TOKEN,        // extern
    START_TOKEN,         // start
    RETURN_TOKEN,        // return
    EXIT_TOKEN,          // exit
    RETURN_TYPE_TOKEN,   // =>
    SCOPE_TOKEN,         // {  }

    // Function
    ASM_TOKEN,           // asm
    SYSCALL_TOKEN,       // syscall
    EXFUNC_TOKEN,        // exfunc
    FUNC_TOKEN,          // function
    FUNC_NAME_TOKEN,
    CALL_TOKEN,
    
    // Condition scope
    SWITCH_TOKEN,        // switch
    CASE_TOKEN,          // case
    DEFAULT_TOKEN,       // default
    WHILE_TOKEN,         // while
    IF_TOKEN,            // if
    ELSE_TOKEN,          // else
    
    // Statements
    PLUS_TOKEN,          // +
    MINUS_TOKEN,         // -
    MULTIPLY_TOKEN,      // *
    DIVIDE_TOKEN,        // /
    MODULO_TOKEN,        // %
    ADDASSIGN_TOKEN,     // +=
    SUBASSIGN_TOKEN,     // -=
    MULASSIGN_TOKEN,     // *=
    DIVASSIGN_TOKEN,     // /=
    ASSIGN_TOKEN,        // =
    COMPARE_TOKEN,       // ==
    NCOMPARE_TOKEN,      // !=
    LOWER_TOKEN,         // <
    LOWEREQ_TOKEN,       // <=
    LARGER_TOKEN,        // >
    LARGEREQ_TOKEN,      // >=
    BITMOVE_LEFT_TOKEN,  // <<
    BITMOVE_RIGHT_TOKEN, // >>
    BITAND_TOKEN,        // &
    BITOR_TOKEN,         // |
    BITXOR_TOKEN,        // ^
    AND_TOKEN,           // &&
    OR_TOKEN,            // ||
    
    // Vars
    F64_VARIABLE_TOKEN,  // f64
    F32_VARIABLE_TOKEN,  // f32
    I64_VARIABLE_TOKEN,  // i64
    I32_VARIABLE_TOKEN,  // i32
    I16_VARIABLE_TOKEN,  // i16
    I8_VARIABLE_TOKEN,   // i8
    U64_VARIABLE_TOKEN,  // u64
    U32_VARIABLE_TOKEN,  // u32
    U16_VARIABLE_TOKEN,  // u16
    U8_VARIABLE_TOKEN,   // u8
    STR_VARIABLE_TOKEN,  // str
    ARR_VARIABLE_TOKEN,  // arr

    // Values
    STRING_VALUE_TOKEN,
    CHAR_VALUE_TOKEN,

    // debug
    BREAKPOINT_TOKEN, // lis
} token_type_t;

typedef struct {
    char ro;   /* Is read only flag   */
    char glob; /* Is global flag      */
    char ptr;  /* Is pointer flag     */
    char ref;  /* Is reference flag   */
    char dref; /* Is dereference flag */
    char ext;  /* Is extern flag      */
    char neg;  /* Is negative flag    */
    char heap; /* Is heap allocated   */
} token_flags_t;

#define TOKEN_MAX_SIZE 128
typedef struct {
    token_flags_t flags;
    token_type_t  t_type;
    char          value[TOKEN_MAX_SIZE];
    int           lnum; /* Line in source file */
} token_t;

token_type_t TKN_get_tmp_type(token_type_t t);
int TKN_istmp_type(token_type_t t);
int TKN_variable_bitness(token_t* token, char ptr);
int TKN_isptr(token_t* token);
int TKN_one_slot(token_t* token);
int TKN_instack(token_t* token);
int TKN_isblock(token_t* token);
int TKN_isdecl(token_t* token);
int TKN_isclose(token_t* token);
int TKN_isoperand(token_t* token);
int TKN_token_priority(token_t* token);
int TKN_isnumeric(token_t* token);
int TKN_isvariable(token_t* token);
int TKN_issign(token_t* token);
int TKN_is_float(token_t* token);
int TKN_update_operator(token_t* token);

#endif