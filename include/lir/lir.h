#ifndef LIR_H_
#define LIR_H_

#include <ast/ast.h>
#include <ast/syntax.h>
#include <lir/lir_types.h>
#include <std/mm.h>
#include <std/str.h>
#include <std/regs.h>
#include <std/vars.h>
#include <std/stack.h>

#define LIR_VAL_MSIZE 128
typedef struct {
    long value;
} lir_constant_t;

typedef struct {
    int offset;
} lir_variable_t;

typedef struct {
    registers_t reg;
} lir_register_t;

typedef struct {
    char value[LIR_VAL_MSIZE];
} lir_number_t;

typedef struct {
    long               id;
    char               size;
    lir_subject_type_t t;
    union {
        lir_constant_t cnst;
        lir_variable_t var;
        lir_register_t reg;
        lir_number_t   num;
    } storage;
} lir_subject_t;

typedef enum {
    /* Operations */
        /* x86_64 ASM commands */
        LIR_MKGLB, // global keyword

        /* Commands */
        LIR_FCLL, // function call
        LIR_ECLL, // extern function call
        LIR_STRT, // start macro
        LIR_SYSC, // syscall
        LIR_FRET, // function ret
        LIR_TDBL, // convert to double
        LIR_TST,  // test
        LIR_XCHG, // xchg
        LIR_CDQ,  // cdq
        LIR_MKLB, // mk label
        LIR_FDCL, // declare function
        LIR_OEXT, // extern object

        LIR_SETL,
        LIR_SETG,
        LIR_STLE,
        LIR_STGE,
        LIR_SETE,
        LIR_STNE,
        LIR_SETB,
        LIR_SETA,
        LIR_STBE,
        LIR_STAE,

        /* Data commands */
        LIR_RESV,
        LIR_VDCL,

        /* Jump instructions */
        LIR_JMP,  // jmp
        LIR_JE,
        LIR_JNE,
        LIR_JL,
        LIR_JG,

    /* Register */
        /* Operations */
        LIR_iMOV, // integer move
        LIR_iMOVb, // mov byte
        LIR_iMOVw, // mov word
        LIR_iMOVd, // mov dword
        LIR_iMOVq, // mov qword
        LIR_iMVZX,

        LIR_fMOV, // float move
        LIR_fMVf, // float to float move
        LIR_REF,  // lea move
        LIR_GDREF, // get value from address
        LIR_LDREF, // set valye by address
        LIR_PUSH, // push
        LIR_POP,  // pop

    /* Integer */
        /* Binary operations */
        LIR_iADD, // addition
        LIR_iSUB, // substraction
        LIR_iMUL, // multiplication
        LIR_DIV,
        LIR_iDIV, // division
        LIR_iMOD, // module
        LIR_iLRG, // larger
        LIR_iLGE, // larger or equals
        LIR_iLWR, // lower
        LIR_iLRE, // lower or equals
        LIR_iCMP, // comprarision
        LIR_iNMP,

        /* Logic */
        LIR_iAND, // and
        LIR_iOR,  // or

    /* Float */
        /* Binary operations */
        LIR_fADD, // addition
        LIR_fSUB, // substruction
        LIR_fMUL, // multiplication
        LIR_fDIV, // division
        LIR_fCMP, // cmp for double

    /* Bits */
        /* Binary operations */
        LIR_iBLFT, // bit left
        LIR_iBRHT, // bit right
        LIR_bAND, // bit and
        LIR_bOR,  // bit or
        LIR_bXOR, // bit xor
        LIR_bSHL, // bitleft
        LIR_bSHR, // bitright
        LIR_bSAR, // bitleft unsgn

    /* Other */
    LIR_RAW,

    /* High level operations */
        /* Stack */
        LIR_RSVSTK, // Reserve stackframe
    
        /* Bin operations */
        LIR_ADDOP,  // a = b + c
        LIR_fADDOP, // fa = f/b + f/c
        LIR_SUBOP,  // a = b - c
        LIR_fSUBOP, // fa = f/b - f/c
        LIR_DIVOP,  // a = b / c
        LIR_fDIVOP, // fa = f/b / f/c
        LIR_MODOP,  // a = b % c

        /* Data */
        LIR_LOADOP, // load value <= a
        LIR_LDLINK, // load link <= a
        LIR_STOP,   // store value a =>
        LIR_STLINK, // store link a =>
        LIR_DECL,   // declaration
    
        /* Heap */
        LIR_ALLCH,  // allocate heap + save addr to farg
        LIR_DEALLH, // deallocate heap, addr from farg

        /* System */
        LIR_EXITOP, // Exit with farg exit call
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
    scope_stack_t heap;
} lir_ctx_t;

lir_subject_t* LIR_create_subject(
    int t, registers_t r, 
    int v_id, long offset, const char* strval, 
    long intval, int size, int s_id
);

lir_ctx_t* LIR_create_ctx();
lir_block_t* LIR_create_block(lir_operation_t op, lir_subject_t* fa, lir_subject_t* sa, lir_subject_t* ta);
int LIR_append_block(lir_block_t* block, lir_ctx_t* ctx);
int LIR_remove_block(lir_block_t* block, lir_ctx_t* ctx);
int LIR_unload_blocks(lir_block_t* block);
int LIR_destroy_ctx(lir_ctx_t* ctx);

#define LIR_SUBJ_REG(r, sz) \
    LIR_create_subject(LIR_REGISTER, r, -1, 0, NULL, 0, sz, 0)

#define LIR_SUBJ_CONST(val) \
    LIR_create_subject(LIR_CONSTVAL, 0, -1, 0, NULL, val, 0, 0)

#define LIR_SUBJ_NUMBER(val) \
    LIR_create_subject(LIR_NUMBER, 0, -1, 0, val, 0, 0, 0)

#define LIR_SUBJ_VAR(kind, off, sz) \
    LIR_create_subject(kind, 0, -1, off, NULL, 0, sz, 0)

#define LIR_SUBJ_GLVAR(id) \
    LIR_create_subject(LIR_GLVARIABLE, 0, id, 0, NULL, 0, 0, 0)

#define LIR_SUBJ_OFF(off, sz) \
    LIR_create_subject(LIR_MEMORY, 0, -1, off, NULL, 0, sz, 0)

#define LIR_SUBJ_LABEL(id) \
    LIR_create_subject(LIR_LABEL, 0, id, 0, NULL, 0, 0, 0)

#define LIR_SUBJ_RAWASM(l) \
    LIR_create_subject(LIR_RAWASM, 0, l, 0, NULL, 0, 0, 0)

#define LIR_SUBJ_STRING(id) \
    LIR_create_subject(LIR_FNAME, 0, id, 0, NULL, 0, 0, -1)

#define LIR_SUBJ_FUNCNAME(n) \
    LIR_create_subject(LIR_FNAME, 0, n->storage.str.s_id, 0, NULL, 0, 0, -1)

#define LIR_BLOCK0(ctx, op) \
    LIR_append_block(LIR_create_block((op), NULL, NULL, NULL), (ctx))

#define LIR_BLOCK1(ctx, op, fa) \
    LIR_append_block(LIR_create_block((op), (fa), NULL, NULL), (ctx))

#define LIR_BLOCK2(ctx, op, fa, sa) \
    LIR_append_block(LIR_create_block((op), (fa), (sa), NULL), (ctx))

#endif