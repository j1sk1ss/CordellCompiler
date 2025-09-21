#ifndef REGS_H_
#define REGS_H_

#include <std/str.h>
#include <std/logg.h>
#include <prep/token.h>

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

typedef enum {
    XMM0, XMM1,
    RAX, RBX, RCX, RDX, RSI, RDI, RBP, RSP, R8, R9, R10,
    EAX, EBX, ECX, EDX, ESI, EDI, EBP, ESP, 
    AX,  BX,  CX,  DX,  
    AL,  BL,  CL,  DL,
    AH,  BH,  CH,  DH
} registers_t;

static inline char* format_from_stack(int offset) {
    static char stack_buff[64] = { 0 };
    snprintf(stack_buff, sizeof(stack_buff), "[rbp - %d]", offset);
    return stack_buff;
}

static inline char* format_from_data(token_t* tkn) {
    static char data_buff[64] = { 0 };
    if (tkn->t_type == UNKNOWN_NUMERIC_TOKEN) return tkn->value;
    else {
        if (
            tkn->t_type == ARR_VARIABLE_TOKEN || 
            tkn->t_type == STR_VARIABLE_TOKEN
        ) snprintf(data_buff, sizeof(data_buff), tkn->flags.ext ? "%s" : "[rel __%s__]", tkn->value);
        else snprintf(data_buff, sizeof(data_buff), tkn->flags.ext ? "[rel %s]" : "[rel __%s__]", tkn->value);
        return data_buff;
    }
}

#define GET_ASMVAR(node) \
    VRS_instack((node)->token) ? \
    format_from_stack((node)->sinfo.v_id) : \
    format_from_data((node)->token)

#endif