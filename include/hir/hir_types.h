#ifndef HIR_TYPES_H_
#define HIR_TYPES_H_

#include <prep/token.h>
#include <std/vars.h>

typedef enum hir_operation {
    /* Operations */
        /* Commands */
        HIR_FCLL,  // function call
        HIR_ECLL,  // extern function call
        HIR_STRT,  // start macro
        HIR_STEND, // end macro
        HIR_SYSC,  // syscall
        HIR_FRET,  // function ret
        HIR_TDBL,  // convert to double
        HIR_MKLB,  // mk label
        HIR_FDCL,  // declare function
        HIR_FEND,  // function end
        HIR_OEXT,  // extern object

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

typedef enum hir_subject_type {
    HIR_TMPVARSTR,
    HIR_TMPVARARR,
    HIR_TMPVARF64,
    HIR_TMPVARU64,
    HIR_TMPVARI64,
    HIR_TMPVARF32,
    HIR_TMPVARU32,
    HIR_TMPVARI32,
    HIR_TMPVARU16,
    HIR_TMPVARI16,
    HIR_TMPVARU8,
    HIR_TMPVARI8,
    HIR_GLBVARSTR,
    HIR_GLBVARARR,
    HIR_GLBVARF64,
    HIR_GLBVARU64,
    HIR_GLBVARI64,
    HIR_GLBVARF32,
    HIR_GLBVARU32,
    HIR_GLBVARI32,
    HIR_GLBVARU16,
    HIR_GLBVARI16,
    HIR_GLBVARU8,
    HIR_GLBVARI8,
    HIR_STKVARSTR,
    HIR_STKVARARR,
    HIR_STKVARF64,
    HIR_STKVARU64,
    HIR_STKVARI64,
    HIR_STKVARF32,
    HIR_STKVARU32,
    HIR_STKVARI32,
    HIR_STKVARU16,
    HIR_STKVARI16,
    HIR_STKVARU8,
    HIR_STKVARI8, // var.id
    HIR_CONSTVAL, // cnst.value
    HIR_NUMBER,   // num.value
    HIR_LABEL,    // id
    HIR_RAWASM,   // str.id
    HIR_STRING,   // str.id
    HIR_FNAME,    // str.id
} hir_subject_type_t;

int HIR_get_type_size(hir_subject_type_t t);
hir_subject_type_t HIR_promote_types(hir_subject_type_t a, hir_subject_type_t b);
hir_subject_type_t HIR_get_tmptype_tkn(token_t* token);
hir_subject_type_t _get_glbtype(int bitness, int isfloat, int issigned);
hir_subject_type_t HIR_get_stktype(token_t* token);
hir_subject_type_t HIR_get_tmp_type(hir_subject_type_t t);
int HIR_isleader(hir_operation_t op);
int HIR_isjmp(hir_operation_t op);
int HIR_iscondjmp(hir_operation_t op);

#endif