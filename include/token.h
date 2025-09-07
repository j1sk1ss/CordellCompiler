#ifndef TKNZ_H_
#define TKNZ_H_

#include <stddef.h>
#include <unistd.h>
#include <fcntl.h>
#include <ctype.h>

#include "mm.h"
#include "str.h"

#define TOKEN_MAX_SIZE 128
#define BUFFER_SIZE    8192

typedef enum {
    // Unknowns
    UNKNOWN_CHAR_TOKEN,
    UNKNOWN_BRACKET_VALUE,
    UNKNOWN_STRING_TOKEN,
    UNKNOWN_NUMERIC_TOKEN,
    UNKNOWN_COMMAND_TOKEN,
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

    // Types
    DREF_TYPE_TOKEN,  // dref
    REF_TYPE_TOKEN,   // ref
    PTR_TYPE_TOKEN,   // ptr
    RO_TYPE_TOKEN,    // ro
    GLOB_TYPE_TOKEN,  // glob
    LONG_TYPE_TOKEN,
    INT_TYPE_TOKEN,
    SHORT_TYPE_TOKEN,
    CHAR_TYPE_TOKEN,
    STR_TYPE_TOKEN,
    ARRAY_TYPE_TOKEN,
    
    // Commands
    IMPORT_TOKEN,        // import
    IMPORT_SELECT_TOKEN, // from
    EXTERN_TOKEN,        // extern
    START_TOKEN,         // start
    RETURN_TOKEN,        // return
    EXIT_TOKEN,          // exit
    SCOPE_TOKEN,

    // Function
    SYSCALL_TOKEN,       // syscall
    EXFUNC_TOKEN,
    FUNC_TOKEN,          // function
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
    ASSIGN_TOKEN,         // =
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
    LONG_VARIABLE_TOKEN,  // long
    INT_VARIABLE_TOKEN,   // int
    SHORT_VARIABLE_TOKEN, // short
    CHAR_VARIABLE_TOKEN,  // char
    STR_VARIABLE_TOKEN,   // str
    ARR_VARIABLE_TOKEN,   // arr

    // Values
    STRING_VALUE_TOKEN,
    CHAR_VALUE_TOKEN
} token_type_t;

typedef struct {
    char ro;   /* Is read only flag   */
    char glob; /* Is global flag      */
    char ptr;  /* Is pointer flag     */
    char ref;  /* Is reference flag   */
    char dref; /* Is dereference flag */
    char ext;  /* Is extern flag      */
} tkn_var_info_t;

typedef struct token {
    tkn_var_info_t vinfo;
    token_type_t   t_type;
    char           value[TOKEN_MAX_SIZE];
    struct token*  next;
    int            lnum; /* Line in source file */
} token_t;

/*
Allocate and create token.
Params:
    - type - Token type.
    - value - Token content.
    - len - Value variable size.
    - line - Token line.

Return pointer to token, or NULL if malloc error.
*/
token_t* TKN_create_token(token_type_t type, const char* value, size_t len, int line);

/*
Tokenize input file by provided FD.
Params:
    - fd - File descriptor of target file.

Return NULL or pointer to linked list of tokens.
Note: Function don't close file.
*/
token_t* TKN_tokenize(int fd);

/*
Unload linked list of tokens.
Params:
    - head - Linked list head.

Return 1 if unload success.
*/
int TKN_unload(token_t* head);

#endif