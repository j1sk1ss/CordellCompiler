#ifndef HIR_H_
#define HIR_H_

#include <std/mm.h>
#include <std/str.h>
#include <std/regs.h>
#include <std/vars.h>
#include <std/stack.h>
#include <symtab/symtab.h>
#include <ast/ast.h>
#include <ast/syntax.h>
#include <hir/hir_types.h>

#define HIR_VAL_MSIZE 128
typedef struct {
    int s_id; /* String ID from strings symtab */
} hir_string_t;

typedef struct {
    long value;
} hir_constant_t;

typedef struct {
     char value[HIR_VAL_MSIZE];
} hir_number_t;

typedef struct {
    int v_id; /* Variable ID from variables symtab */
} hir_variable_t;

typedef struct {
    long               id;
    int                t;
    union {
        hir_string_t   str;
        hir_constant_t cnst;
        hir_number_t   num;
        hir_variable_t var;
    } storage;
} hir_subject_t;

typedef enum {
    /* Operations */
        /* Commands */
        HIR_FCLL, // function call
        HIR_ECLL, // extern function call
        HIR_STRT, // start macro
        HIR_SYSC, // syscall
        HIR_FRET, // function ret
        HIR_TDBL, // convert to double
        HIR_MKLB, // mk label
        HIR_FDCL, // declare function
        HIR_FEND, // function end
        HIR_OEXT, // extern object

        /* Jump instructions */
        HIR_JMP,

    /* Integer */
        /* Binary operations */
        HIR_iADD, // addition
        HIR_iSUB, // substraction
        HIR_iMUL, // multiplication
        HIR_iDIV, // division
        HIR_iMOD, // module
        HIR_iLRG, // larger,            x = y > z
        HIR_iLGE, // larger or equals,  x = y >= z
        HIR_iLWR, // lower,             x = y < z
        HIR_iLRE, // lower or equals,   x = y <= z
        HIR_iCMP, // comprarision,      x = x == y
        HIR_iNMP, // ncomprarision      x = x != y

        /* Logic */
        HIR_iAND, // and
        HIR_iOR,  // or

    /* Bits */
        /* Binary operations */
        HIR_iBLFT, // bit left
        HIR_iBRHT, // bit right
        HIR_bAND, // bit and
        HIR_bOR,  // bit or
        HIR_bXOR, // bit xor
        HIR_bSHL, // bitleft
        HIR_bSHR, // bitright
        HIR_bSAR, // bitleft unsgn

    /* Other */
    HIR_RAW,
    HIR_STASM,
    HIR_ENDASM,

    /* High level operations */
        /* Condition operator */
        HIR_IFOP,     // if X, jmp [lb]
        HIR_IFLGOP,   // if X > Y, jmp [lb]
        HIR_IFLGEOP,  // if X >= Y, jmp [lb]
        HIR_IFLWOP,   // if X < Y, jmp [lb]
        HIR_IFLWEOP,  // if X <= Y, jmp [lb]
        HIR_IFCPOP,   // if X == Y, jmp [lb]
        HIR_IFNCPOP,  // if X != Y, jmp [lb]

        HIR_SWITCHOP,
        HIR_MKCASE,
        HIR_MKDEFCASE,
        HIR_MKENDCASE,

        /* Data */
        HIR_NOT,
        HIR_LOADOP,  // load value <= a
        HIR_LDLINK,  // load link <= a
        HIR_STORE,   // store value a =>
        HIR_STLINK,  // store link a =>
        HIR_CLNVRS,  // deallocate all unused variables
        HIR_VARDECL, // declaration
        HIR_ARRDECL, // arr declaration
        HIR_STRDECL,
        HIR_PRMST,
        HIR_PRMLD,   // load param
        HIR_PRMPOP,  // pop params
        HIR_FARGST,  // store function argument
        HIR_FARGLD,  // load function argument
        HIR_STARGLD, // load start argument
        HIR_GINDEX,  // get data by index in array
        HIR_LINDEX,  // load data to array by index
        HIR_GDREF,   // get value by address
        HIR_LDREF,   // load value to address
        HIR_REF,
        HIR_IMPORT,
        HIR_EXTERN,
        
        /* System */
        HIR_EXITOP, // Exit with farg exit call

        HIR_MKSCOPE,  // scope
        HIR_ENDSCOPE, // end of scope
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
    hir_block_t* h;
    hir_block_t* t;
} hir_ctx_t;

hir_subject_t* HIR_create_subject(
    int t, registers_t r, 
    int v_id, const char* strval, 
    long intval, int s_id
);

hir_ctx_t* HIR_create_ctx();
hir_block_t* HIR_create_block(hir_operation_t op, hir_subject_t* fa, hir_subject_t* sa, hir_subject_t* ta);
int HIR_insert_block(hir_block_t* block, hir_block_t* pos);
int HIR_append_block(hir_block_t* block, hir_ctx_t* ctx);
int HIR_remove_block(hir_block_t* block, hir_ctx_t* ctx);
int HIR_unload_blocks(hir_block_t* block);
int HIR_destroy_ctx(hir_ctx_t* ctx);

#define HIR_SUBJ_CONST(val) \
    HIR_create_subject(HIR_CONSTVAL, 0, 0, NULL, val, -1)

#define HIR_SUBJ_NUMBER(val) \
    HIR_create_subject(HIR_NUMBER, 0, 0, val, 0, -1)

#define HIR_SUBJ_STKVAR(v_id, kind, s_id) \
    HIR_create_subject(kind, 0, v_id, NULL, 0, s_id)

#define HIR_SUBJ_GLBVAR(name, kind) \
    HIR_create_subject(kind, 0, 0, name, 0, -1)

#define HIR_SUBJ_VAR(n) HIR_SUBJ_STKVAR(n->sinfo.v_id, HIR_get_stktype(n->token), n->sinfo.s_id)

#define HIR_SUBJ_TMPVAR(kind, id) \
    HIR_create_subject(HIR_get_tmp_type(kind), 0, id, NULL, 0, -1)

#define HIR_SUBJ_LABEL() \
    HIR_create_subject(HIR_LABEL, 0, 0, NULL, 0, -1)

#define HIR_SUBJ_RAWASM(n) \
    HIR_create_subject(HIR_RAWASM, 0, n->sinfo.v_id, NULL, 0, -1)

#define HIR_SUBJ_STRING(n) \
    HIR_create_subject(HIR_STRING, 0, n->sinfo.v_id, NULL, 0, -1)

#define HIR_SUBJ_FUNCNAME(n) \
    HIR_create_subject(HIR_FNAME, 0, n->sinfo.v_id, NULL, 0, -1)

#define HIR_BLOCK0(ctx, op) \
    HIR_append_block(HIR_create_block((op), NULL, NULL, NULL), (ctx))

#define HIR_BLOCK1(ctx, op, fa) \
    HIR_append_block(HIR_create_block((op), (fa), NULL, NULL), (ctx))

#define HIR_BLOCK2(ctx, op, fa, sa) \
    HIR_append_block(HIR_create_block((op), (fa), (sa), NULL), (ctx))

#define HIR_BLOCK3(ctx, op, fa, sa, ta) \
    HIR_append_block(HIR_create_block((op), (fa), (sa), (ta)), (ctx))

#endif