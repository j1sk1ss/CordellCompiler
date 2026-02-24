#ifndef TOKEN_TYPES_H_
#define TOKEN_TYPES_H_

#include <std/str.h>

typedef enum {
    // Pre-processor tokens
    PP_TOKEN,  // #file "file"
               // for instance: #file "something.cpl"
    /* This token tells us about current info for the 
       finfo structure in the rokenizer. */
    INCLUDE_FILE_TOKEN,

    // Unknowns
    UNKNOWN_CHAR_TOKEN,
    UNKNOWN_BRACKET_VALUE,
    UNKNOWN_STRING_TOKEN,
    UNKNOWN_NUMERIC_TOKEN,       // 123
    UNKNOWN_FLOAT_NUMERIC_TOKEN, // 123.123
    UNKNOWN_SIGN_TOKEN,          // +, -, *, /, etc
    LINE_BREAK_TOKEN,

    DELIMITER_TOKEN,     // ;
    COMMA_TOKEN,         // ,
    INDEXATION_TOKEN,    // []
    CALLING_TOKEN,       // ()
    OPEN_INDEX_TOKEN,    // [
    CLOSE_INDEX_TOKEN,   // ]
    OPEN_BRACKET_TOKEN,  // (
    CLOSE_BRACKET_TOKEN, // )
    OPEN_BLOCK_TOKEN,    // {
    CLOSE_BLOCK_TOKEN,   // }

    // Modifiers
    DREF_TYPE_TOKEN,     // dref
    REF_TYPE_TOKEN,      // ref
    PTR_TYPE_TOKEN,      // ptr
    RO_TYPE_TOKEN,       // ro
    GLOB_TYPE_TOKEN,     // glob
    NEGATIVE_TOKEN,      // not

    // Data types
    VAR_ARGUMENTS_TOKEN, // ...
    TMP_TYPE_TOKEN,      // tmp
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

    I0_TYPE_TOKEN,       // i0
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
    
    // Convert statements
    CONVERT_TOKEN,       // as

    // Commands
    IMPORT_TOKEN,        // import
    IMPORT_SELECT_TOKEN, // from
    EXTERN_TOKEN,        // extern
    START_TOKEN,         // start
    RETURN_TOKEN,        // return
    EXIT_TOKEN,          // exit
    RETURN_TYPE_TOKEN,   // ->
    SCOPE_TOKEN,         // {  }

    // Function
    ASM_TOKEN,           // asm
    SYSCALL_TOKEN,       // syscall
    EXFUNC_TOKEN,        // exfunc
    FUNC_PROT_TOKEN,     // function <name> - prototype
    FUNC_TOKEN,          // function
    FUNC_NAME_TOKEN,     // function <name>
    CALL_TOKEN,          // fname(...)
    ADDR_CALL_TOKEN,     // something(...) - doesn't support default args, etc, but can handle addr to anything
    CALL_ADDR,           // fname without () operation. Means that we're working with the address of a function
    
    // Condition scope
    SWITCH_TOKEN,        // switch
    CASE_TOKEN,          // case
    DEFAULT_TOKEN,       // default
    WHILE_TOKEN,         // while
    LOOP_TOKEN,          // loop
    BREAK_TOKEN,         // break
    IF_TOKEN,            // if
    ELSE_TOKEN,          // else
    POPARG_TOKEN,        // poparg
    
    // Operands
    PLUS_TOKEN,          // +
    MINUS_TOKEN,         // -
    MULTIPLY_TOKEN,      // *
    DIVIDE_TOKEN,        // /
    MODULO_TOKEN,        // %
    ADDASSIGN_TOKEN,     // +=
    SUBASSIGN_TOKEN,     // -=
    MULASSIGN_TOKEN,     // *=
    DIVASSIGN_TOKEN,     // /=
    MODULOASSIGN_TOKEN,  // %=
    BITANDASSIGN_TOKEN,  // &=
    BITORASSIGN_TOKEN,   // |=
    BITXORASSIGN_TOKEN,  // ^=
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
    
    // Variables (not a type, a variable)
    VARIABLE_TOKEN,      // front-end tokenizer variable abstraction
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
    STRING_VALUE_TOKEN,  // "something"
    CHAR_VALUE_TOKEN,    // 's'

    // Debug statements
    BREAKPOINT_TOKEN, // lis
} token_type_t;

typedef struct {
    char ro   : 1; /* Is read only flag   */
    char glob : 1; /* Is global flag      */
    int  ptr;      /* Is pointer flag     */
    char ext  : 1; /* Is extern flag      */
    char heap : 1; /* Is heap allocated   */
} token_flags_t;

typedef struct {
    long      line;
    long      column;
    string_t* file;
} token_fpos_t;

typedef struct {
    token_flags_t flags;  /* Token's flags           */
    token_type_t  t_type; /* Token's type            */
    string_t*     body;   /* Token's body            */
    token_fpos_t  finfo;  /* Source file information */
} token_t;

token_type_t TKN_get_tmp_type(token_type_t t);
int TKN_istmp_type(token_type_t t);

typedef enum {
    TYPE_FULL_SIZE    = 4, /* For instance x86_64: 64-bit */
    TYPE_HALF_SIZE    = 3, /* 32-bit                      */
    TYPE_QUARTER_SIZE = 2, /* 16-bit                      */
    TYPE_EIGHTH_SIZE  = 1  /* 8-bit                       */
} type_size_t;

type_size_t TKN_variable_bitness(token_t* token, char ptr);
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
int TKN_issign(token_t* token, char ptr);
int TKN_is_float(token_t* token);
int TKN_update_operator(token_t* token);
token_type_t TKN_get_var_from_type(token_type_t t);

#endif
