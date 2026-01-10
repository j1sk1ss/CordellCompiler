#ifndef AST_DATA_H_
#define AST_DATA_H_

typedef enum {
    EXPRESSION_NODE  = 1 << 0,
    ASSIGN_NODE      = 1 << 1,
    DECLARATION_NODE = 1 << 2,
    FUNCTION_NODE    = 1 << 3,
    CALL_NODE        = 1 << 4,
    START_NODE       = 1 << 5,
    DEF_ARRAY_NODE   = 1 << 6,
    IF_NODE          = 1 << 7,
    WHILE_NODE       = 1 << 8,
    LOOP_NODE        = 1 << 9,
    TERM_NODE        = 1 << 10,
    SWITCH_NODE      = 1 << 11,
    CASE_NODE        = 1 << 12,
    BREAK_NODE       = 1 << 13,
    UNKNOWN_NODE     = 1 << 14,
} ast_node_type_t;

typedef enum {
    ATTENTION_UNKNOWN_LEVEL, /* Unknown attention level. Will fire a warning only with all warns enabled  */
    ATTENTION_LOW_LEVEL,     /* Low level of an attantion. Will fire a warning with a low level enabled   */
    ATTENTION_MEDIUM_LEVEL,  /* Mid level of an attantion. Will fire a warning with a mid level enabled   */
    ATTENTION_HIGH_LEVEL,    /* High level of an attantion. Will fire a warning with a high level enabled */
    ATTENTION_BLOCK_LEVEL    /* Will block the code compilation if it fires                               */
} attention_level_t;

typedef struct {
    int in_switch; /* Walker in a switch block    */
    int in_case;   /* Walker in a case block      */
    int in_while;  /* Walker in a while block     */
    int in_loop;   /* Walker in a loop block      */
    int in_if;     /* Walker in an if block       */
    int in_func;   /* Walker in a function        */
    int in_start;  /* Walker in a start function  */
    int in_fargs;  /* Walker in a function params */
    int in_stargs; /* Walker in a start params    */
} walker_flags_t;

#define AST_VISITOR_ARGS     ast_node_t* nd, walker_flags_t* flags, sym_table_t* smt
#define AST_VISITOR_ARGS_USE (void*)nd; (void*)flags; (void*)smt;

#endif