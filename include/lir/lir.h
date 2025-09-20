#ifndef LIR_H_
#define LIR_H_

#include <ast/ast.h>
#include <ast/syntax.h>
#include <std/mm.h>
#include <std/str.h>
#include <std/regs.h>
#include <std/vars.h>
#include <std/stack.h>

#define IR_VAL_MSIZE 128
typedef struct {
    char value[IR_VAL_MSIZE];
} lir_string_t;

typedef struct {
    long value;
} lir_constant_t;

typedef struct {
    int offset;
} lir_variable_t;

typedef struct {
    registers_t reg;
} lir_register_t;

typedef enum {
    REGISTER,
    STVARIABLE,
    GLVARIABLE,
    CONSTVAL,
    LABEL,
    RAWASM,
} lir_subject_type_t;

typedef struct {
    long               id;
    char               size;
    char               dref;
    char               ref;
    lir_subject_type_t t;
    union {
        lir_string_t   str;
        lir_constant_t cnst;
        lir_variable_t var;
        lir_register_t reg;
    } storage;
} lir_subject_t;

typedef enum {
    /* Operations */
        /* Commands */
        FCLL, // function call
        ECLL, // extern function call
        STRT, // start macro
        SYSC, // syscall
        FRET, // function ret
        TDBL, // convert to double
        TST,  // test
        XCHG, // xchg
        CDQ,  // cdq
        MKLB, // mk label
        FDCL, // declare function
        OEXT, // extern object

        SETL,
        SETG,
        STLE,
        STGE,
        SETE,
        STNE,
        SETB,
        SETA,
        STBE,
        STAE,

        /* Data commands */
        RESV,
        VDCL,

        /* Jump instructions */
        JMP,  // jmp
        JE,
        JNE,
        JL,
        JG,

    /* Register */
        /* Operations */
        iMOV, // integer move
        iMOVb, // mov byte
        iMOVw, // mov word
        iMOVd, // mov dword
        iMOVq, // mov qword
        iMVZX,

        fMOV, // float move
        fMVf, // float to float move
        LEA,  // lea move
        PUSH, // push
        POP,  // pop

    /* Integer */
        /* Binary operations */
        iADD, // addition
        iSUB, // substraction
        iMUL, // multiplication
        DIV,
        iDIV, // division
        iMOD, // module
        iLRG, // larger
        iLGE, // larger or equals
        iLWR, // lower
        iLRE, // lower or equals
        iCMP, // comprarision
        iNMP,

        /* Logic */
        iAND, // and
        iOR,  // or

    /* Float */
        /* Binary operations */
        fADD, // addition
        fSUB, // substruction
        fMUL, // multiplication
        fDIV, // division
        fCMP, // cmp for double

    /* Bits */
        /* Binary operations */
        iBLFT, // bit left
        iBRHT, // bit right
        bAND, // bit and
        bOR,  // bit or
        bXOR, // bit xor
        bSHL, // bitleft
        bSHR, // bitright
        bSAR, // bitleft unsgn

    /* Other */
    RAW,

    /* High level operations */
        /* Stack */
        RSVSTK, // Reserve stackframe
    
        /* Bin operations */
        ADDOP,  // a = b + c
        fADDOP, // fa = f/b + f/c
        SUBOP,  // a = b - c
        fSUBOP, // fa = f/b - f/c
        DIVOP,  // a = b / c
        fDIVOP, // fa = f/b / f/c
        MODOP,  // a = b % c

        /* Data */
        LOADOP, // load value <= a
        LDLINK, // load link <= a
        STOP,   // store value a =>
        STLINK, // store link a =>
        DECL,   // declaration
    
        /* Heap */
        ALLCH,  // allocate heap + save addr to farg
        DEALLH, // deallocate heap, addr from farg

        /* System */
        EXITOP, // Exit with farg exit call
} lir_operation_t;

typedef struct lir_block {
    struct lir_block* prev;
    struct lir_block* next;
    lir_operation_t   op;
    lir_subject_t*    farg;
    lir_subject_t*    sarg;
    lir_subject_t*    targ;
    int               args;
} lir_block_t;

typedef struct {
    long          cid;
    int           lid;
    lir_block_t*  h;
    lir_block_t*  t;
    syntax_ctx_t* synt;
    scope_stack_t heap;
} lir_ctx_t;

lir_subject_t* IR_create_subject(
    lir_subject_type_t t,
    registers_t r,        
    int dref,             
    int offset,           
    const char* strval,   
    long intval,          
    int size              
);

lir_ctx_t* LIR_create_ctx();
lir_block_t* LIR_create_block(lir_operation_t op, lir_subject_t* fa, lir_subject_t* sa, lir_subject_t* ta);
int LIR_insert_block(lir_block_t* block, lir_ctx_t* ctx);
int LIR_remove_block(lir_block_t* block, lir_ctx_t* ctx);
int LIR_unload_blocks(lir_block_t* block);
int LIR_destroy_ctx(lir_ctx_t* ctx);

static inline const char* _tmp_str(const char* fmt, ...) {
    static char buf[256] = { 0 };
    va_list args;
    va_start(args, fmt);
    vsnprintf(buf, sizeof(buf), fmt, args);
    va_end(args);
    return buf;
}

#define LIR_SUBJ_REG(r, sz) \
    LIR_create_subject(REGISTER, r, 0, 0, NULL, 0, sz)

#define LIR_SUBJ_CONST(val) \
    LIR_create_subject(CONSTVAL, 0, 0, 0, NULL, val, 0)

#define LIR_SUBJ_VAR(off, sz, kind) \
    LIR_create_subject(kind, 0, 0, off, NULL, 0, sz)

#define LIR_SUBJ_LABEL(...) \
    LIR_create_subject(LABEL, 0, 0, 0, _tmp_str(__VA_ARGS__), 0, 0)

#define LIR_SUBJ_RAWASM(l) \
    LIR_create_subject(RAWASM, 0, 0, 0, l, 0, 0)

#define LIR_BLOCK0(ctx, op) \
    LIR_insert_block(LIR_create_block((op), NULL, NULL, NULL), (ctx))

#define LIR_BLOCK1(ctx, op, fa) \
    LIR_insert_block(LIR_create_block((op), (fa), NULL, NULL), (ctx))

#define LIR_BLOCK2(ctx, op, fa, sa) \
    LIR_insert_block(LIR_create_block((op), (fa), (sa), NULL), (ctx))

#define LIR_BLOCK3(ctx, op, fa, sa, ta) \
    LIR_insert_block(LIR_create_block((op), (fa), (sa), (ta)), (ctx))

#endif