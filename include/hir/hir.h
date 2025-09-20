#ifndef HIR_H_
#define HIR_H_

#include <ast/ast.h>
#include <ast/syntax.h>
#include <std/mm.h>
#include <std/str.h>
#include <std/regs.h>
#include <std/vars.h>
#include <std/stack.h>
#include <hir/hir_types.h>

#define IR_VAL_MSIZE 128
typedef struct {
    char value[IR_VAL_MSIZE];
} hir_string_t;

typedef struct {
    long value;
} hir_constant_t;

typedef struct {
     char value[IR_VAL_MSIZE];
} hir_number_t;

typedef struct {
    char name[IR_VAL_MSIZE];
} hir_global_variable_t;

typedef struct {
    int offset;
} hir_variable_t;

typedef struct {
    registers_t reg;
} hir_register_t;

typedef struct {
    long                      id;
    char                      size;
    char                      dref;
    char                      ref;
    int                       t;
    union {
        hir_string_t          str;
        hir_constant_t        cnst;
        hir_number_t          num;
        hir_variable_t        var;
        hir_register_t        reg;
        hir_global_variable_t gvar;
    } storage;
} hir_subject_t;

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
        FEND, // function end
        OEXT, // extern object

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

        /* Condition operator */
        IFOP,
        WHILEOP,
        SWITCHOP,
        MKCASE,
        MKDEFCASE,
        MKENDCASE,

        /* Data */
        NOT,
        LOADOP,  // load value <= a
        LDLINK,  // load link <= a
        STORE,   // store value a =>
        STLINK,  // store link a =>
        VARDECL, // declaration
        ARRDECL, // arr declaration
        STRDECL,
        PRMST,
        PRMLD,   // load param
        FARGST,  // store function argument
        FARGLD,  // load function argument
        STARGLD, // load start argument
        GINDEX,  // get data by index in array
        LINDEX,  // load data to array by index
        GDREF,   // get value by address
        LDREF,   // load value to address
        REF,
        IMPORT,
        EXTERN,
    
        /* Heap */
        ALLCH,  // allocate heap + save addr to farg
        DEALLH, // deallocate heap, addr from farg

        /* System */
        EXITOP, // Exit with farg exit call
} hir_operation_t;

typedef struct hir_block {
    struct hir_block* prev;
    struct hir_block* next;
    hir_operation_t   op;
    hir_subject_t*    farg;
    hir_subject_t*    sarg;
    hir_subject_t*    targ;
    int               args;
} hir_block_t;

typedef struct {
    long          cid;
    int           lid;
    hir_block_t*  h;
    hir_block_t*  t;
    syntax_ctx_t* synt;
} hir_ctx_t;

hir_subject_t* HIR_create_subject(
    int t,
    registers_t r,        
    int dref,             
    int offset,           
    const char* strval,   
    long intval,          
    int size              
);

hir_ctx_t* HIR_create_ctx();
hir_block_t* HIR_create_block(hir_operation_t op, hir_subject_t* fa, hir_subject_t* sa, hir_subject_t* ta);
int HIR_insert_block(hir_block_t* block, hir_ctx_t* ctx);
int HIR_remove_block(hir_block_t* block, hir_ctx_t* ctx);
int HIR_unload_blocks(hir_block_t* block);
int HIR_destroy_ctx(hir_ctx_t* ctx);

#define HIR_SUBJ_REG(r, sz) \
    HIR_create_subject(REGISTER, r, 0, 0, NULL, 0, sz)

#define HIR_SUBJ_CONST(val) \
    HIR_create_subject(CONSTVAL, 0, 0, 0, NULL, val, 0)

#define HIR_SUBJ_NUMBER(val) \
    HIR_create_subject(NUMBER, 0, 0, 0, val, 0, 0)

#define HIR_SUBJ_STKVAR(off, kind) \
    HIR_create_subject(kind, 0, 0, off, NULL, 0, 0)

#define HIR_SUBJ_GLBVAR(name, kind) \
    HIR_create_subject(kind, 0, 0, 0, name, 0, 0)

#define HIR_SUBJ_VAR(n) \
    VRS_instack(n->token) ? \
        HIR_SUBJ_STKVAR(n->sinfo.offset, HIR_get_stktype(n->token)) : \
        HIR_SUBJ_GLBVAR(n->token->value, HIR_get_stktype(n->token))

#define HIR_SUBJ_TMPVAR(kind) \
    HIR_create_subject(HIR_get_tmp_type(kind), 0, 0, 0, NULL, 0, 0)

#define HIR_SUBJ_LABEL() \
    HIR_create_subject(LABEL, 0, 0, 0, NULL, 0, 0)

#define HIR_SUBJ_RAWASM(l) \
    HIR_create_subject(RAWASM, 0, 0, 0, l, 0, 0)

#define HIR_SUBJ_STRING(str) \
    HIR_create_subject(STRING, 0, 0, 0, str, 0, 0)

#define HIR_BLOCK0(ctx, op) \
    HIR_insert_block(HIR_create_block((op), NULL, NULL, NULL), (ctx))

#define HIR_BLOCK1(ctx, op, fa) \
    HIR_insert_block(HIR_create_block((op), (fa), NULL, NULL), (ctx))

#define HIR_BLOCK2(ctx, op, fa, sa) \
    HIR_insert_block(HIR_create_block((op), (fa), (sa), NULL), (ctx))

#define HIR_BLOCK3(ctx, op, fa, sa, ta) \
    HIR_insert_block(HIR_create_block((op), (fa), (sa), (ta)), (ctx))

#endif