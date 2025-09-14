#ifndef DICT_H_
#define DICT_H_

// Misc
#define START_COMMAND           "start"
#define EXIT_COMMAND            "exit"
#define IMPORT_COMMAND          "import"
#define IMPORT_SELECT_COMMAND   "from"
#define EXTERN_COMMAND          "extern"
#define RETURN_TYPE_COMMAND     "=>"

#define OPEN_INDEX              "["
#define CLOSE_INDEX             "]"
#define OPEN_BRACKET            "("
#define CLOSE_BRACKET           ")"

#define OPEN_BLOCK              "{"
#define RETURN_COMMAND          "return"
#define CLOSE_BLOCK             "}"

// Function
#define EXFUNCTION_COMMAND      "exfunc"
#define FUNCTION_COMMAND        "function"

// Modifiers
#define DREF_COMMAND            "dref"
#define REF_COMMAND             "ref"
#define PTR_COMMAND             "ptr"
#define RO_COMMAND              "ro"
#define GLOB_COMMAND            "glob"
#define NEGATIVE_COMMAND        "not"

// Types
#define F64_VARIABLE            "f64"
#define F32_VARIABLE            "f32"
#define F16_VARIABLE            "f16"
#define F8_VARIABLE             "f8"
#define I64_VARIABLE            "i64"
#define I32_VARIABLE            "i32"
#define I16_VARIABLE            "i16"
#define I8_VARIABLE             "i8"
#define U64_VARIABLE            "u64"
#define U32_VARIABLE            "u32"
#define U16_VARIABLE            "u16"
#define U8_VARIABLE             "u8"
#define STR_VARIABLE            "str"
#define ARR_VARIABLE            "arr"

// Commands
#define SWITCH_COMMAND          "switch"
#define CASE_COMMAND            "case"
#define DEFAULT_COMMAND         "default"
#define WHILE_COMAND            "while"
#define IF_COMMAND              "if"
#define ELSE_COMMAND            "else"
#define SYSCALL_COMMAND         "syscall"
#define ASM_COMMAND             "asm"

// Symbols and statements (Operands)
#define ASSIGN_STATEMENT        "="
#define COMPARE_STATEMENT       "=="
#define NCOMPARE_STATEMENT      "!="
#define PLUS_STATEMENT          "+"
#define MINUS_STATEMENT         "-"
#define DIVIDE_STATEMENT        "/"
#define MODULO_STATEMENT        "%"
#define MULTIPLY_STATEMENT      "*"
#define LOWER_STATEMENT         "<"
#define LOWEREQ_STATEMENT       "<="
#define LARGER_STATEMENT        ">"
#define LARGEREQ_STATEMENT      ">="
#define BITMOVE_RIGHT_STATMENT  ">>"
#define BITMOVE_LEFT_STATEMENT  "<<"
#define BITAND_STATEMENT        "&"
#define BITOR_STATEMENT         "|"
#define BITXOR_STATEMENT        "^"
#define AND_STATEMENT           "&&"
#define OR_STATEMENT            "||"

#endif