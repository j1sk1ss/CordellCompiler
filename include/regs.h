#ifndef REGS_H_
#define REGS_H_

#include "str.h"
#include "logg.h"
#include "token.h"

static const char* regs[4][8] = {
    { "rax", "rbx", "rcx", "rdx", "rsi", "rdi", "rbp", "rsp" },
    { "eax", "ebx", "ecx", "edx", "esi", "edi", "ebp", "esp" },
    { "ax",  "bx",  "cx",  "dx",  "si",  "di",  "",    ""    },
    { "al",  "bl",  "cl",  "dl",  "sil", "dil", "",    ""    },
};

static inline const char* __get_operation_type__(int size) {
    switch (size) {
        case 1:  return " byte ";
        case 2:  return " word ";
        default: return " ";
    }
}
#define GET_OPERATION_TYPE(size) __get_operation_type__(size)

#define BIT32           32
#define BIT64           64
#define BASE_BITNESS    BIT64

#if (BASE_BITNESS == BIT64)
    #define SYSCALL "syscall"
    #define ALIGN(x) ((x + 7) & ~(7))
#else
    #define SYSCALL "int 0x80"
    #define ALIGN(x) ((x + 3) & ~(3))
#endif

enum {
    RAX, RBX, RCX, RDX, RSI, RDI, RBP, RSP, R8, R9, R10
};

/*
Simple register getter. For more complex API, use get_reg.
Mapping:
0 - RAX EAX AX AL
1 - RBX EBX BX BL
2 - RCX ECX CX CL
4 - RDX EDX DX DL
5 - RBP EBP .. ..
6 - RSP ESP .. ..
*/
#define GET_REG(source, register) __get_register__(VRS_variable_bitness((node)->token, 1), register)
#define GET_RAW_REG(size, register) __get_register__(size, register)
static inline const char* __get_register__(int size, int pos) {
    switch (size) {
        case 8:  return regs[3][pos];
        case 16: return regs[2][pos];
        case 32: return regs[1][pos];
        case 64:
        default: return regs[0][pos];
    }
}   

static inline char* format_from_stack(int offset) {
    static char stack_buff[64] = { 0 };
    snprintf(stack_buff, sizeof(stack_buff), "[rbp - %d]", offset);
    return stack_buff;
}

static inline char* format_from_data(token_t* tkn) {
    static char data_buff[64] = { 0 };
    if (tkn->t_type == UNKNOWN_NUMERIC_TOKEN) return tkn->value; 
    else if (tkn->t_type == UNKNOWN_CHAR_TOKEN || tkn->t_type == CHAR_VALUE_TOKEN) {
        snprintf(data_buff, sizeof(data_buff), "%i", tkn->value[0]);
        return data_buff;
    }
    else {
        if (
            tkn->t_type == ARR_VARIABLE_TOKEN || 
            tkn->t_type == STR_VARIABLE_TOKEN
        ) snprintf(data_buff, sizeof(data_buff), tkn->vinfo.ext ? "%s" : "[rel __%s__]", tkn->value);
        else snprintf(data_buff, sizeof(data_buff), tkn->vinfo.ext ? "[rel %s]" : "[rel __%s__]", tkn->value);
        return data_buff;
    }
}

#define GET_ASMVAR(node) \
    VRS_instack((node)->token) ? \
    format_from_stack((node)->info.offset) : \
    format_from_data((node)->token)

typedef struct {
    const char* name;
    const char* operation;
    const char* move;
} regs_t;

/*
Fills regs struct by valid data about register.
Params:
- regs - Pointer to struct for storing data.
- size - Variable size for register.
- base64 - Base register (RAX, RBX, ... etc).
- ptr - Is pointer or array?

Return 1 in any case.
*/
int get_reg(regs_t* regs, int size, int reg, int ptr);

#endif