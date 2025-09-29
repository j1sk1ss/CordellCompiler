#ifndef HIR_TYPES_H_
#define HIR_TYPES_H_

#include <prep/token.h>
#include <std/vars.h>

typedef enum hir_operation {
    /* Operations */
        /* Commands */
        HIR_FCLL,        // x()
        HIR_STORE_FCLL,  // x = y()
        HIR_ECLL,        // ext x()
        HIR_STORE_ECLL,  // x = ext y()
        HIR_SYSC,        // syscall()
        HIR_STORE_SYSC,  // x = syscall()

        HIR_TF64,  // x = (f64)y
        HIR_TF32,  // x = (f32)y
        HIR_TI64,  // x = (i64)y
        HIR_TI32,  // x = (i32)y
        HIR_TI16,  // x = (i16)y
        HIR_TI8,   // x = (i8)y
        HIR_TU64,  // x = (u64)y
        HIR_TU32,  // x = (u32)y
        HIR_TU16,  // x = (u16)y
        HIR_TU8,   // x = (u8)y

        HIR_STRT,  // start macro
        HIR_STEND, // end macro
        HIR_FRET,  // function ret       ret x
        HIR_MKLB,  // mk label           id:
        HIR_FDCL,  // declare function   fn [str.id]:
        HIR_FEND,  // function end
        HIR_OEXT,  // extern object      extern [str.id]

        /* Jump instructions */
        HIR_JMP,

    /* Integer */
        /* Binary operations */
        HIR_iADD, // addition           x = y + z
        HIR_iSUB, // substraction       x = y - z
        HIR_iMUL, // multiplication     x = y * z
        HIR_iDIV, // division           x = y / z
        HIR_iMOD, // module             x = y % z
        HIR_iLRG, // larger,            x = y > z
        HIR_iLGE, // larger or equals,  x = y >= z
        HIR_iLWR, // lower,             x = y < z
        HIR_iLRE, // lower or equals,   x = y <= z
        HIR_iCMP, // comprarision,      x = x == y
        HIR_iNMP, // ncomprarision      x = x != y

        /* Logic */
        HIR_iAND, // and                x = y && z
        HIR_iOR,  // or                 x = y || z

    /* Bits */
        /* Binary operations */
        HIR_iBLFT, // bit left          x = y << z
        HIR_iBRHT, // bit right         x = y >> z
        HIR_bAND, // bit and            x = y & z
        HIR_bOR,  // bit or             x = y | z
        HIR_bXOR, // bit xor            x = y ^ z

    /* Other */
    HIR_RAW,
    HIR_STASM,
    HIR_ENDASM,

    /* High level operations */
        /* Condition operator */
        HIR_IFOP,     // if x, jmp z
        HIR_IFLGOP,   // if x > y, jmp z
        HIR_IFLGEOP,  // if x >= y, jmp z
        HIR_IFLWOP,   // if x < y, jmp z
        HIR_IFLWEOP,  // if x <= y, jmp z
        HIR_IFCPOP,   // if x == y, jmp z
        HIR_IFNCPOP,  // if x != y, jmp z

        HIR_PHI,      // base: x, new_var y, set: z (bb, v_id)
        HIR_VRDEALL,  // dealloc x

        /* Data */
        HIR_NOT,
        HIR_STORE,   // x = y
        HIR_CLNVRS,  // deallocate all unused variables
        HIR_VARDECL, // alloc x
        HIR_ARRDECL, // arralloc x, y (size)
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
    HIR_SET,      // set.h
} hir_subject_type_t;

int HIR_get_type_size(hir_subject_type_t t);
hir_subject_type_t HIR_promote_types(hir_subject_type_t a, hir_subject_type_t b);
hir_subject_type_t HIR_get_tmptype_tkn(token_t* token, int ptr);
hir_subject_type_t _get_glbtype(int bitness, int isfloat, int issigned);
hir_subject_type_t HIR_get_stktype(token_t* token);
hir_subject_type_t HIR_get_tmp_type(hir_subject_type_t t);
int HIR_isleader(hir_operation_t op);
int HIR_isjmp(hir_operation_t op);
int HIR_iscondjmp(hir_operation_t op);
int HIR_is_vartype(hir_subject_type_t t);
int HIR_is_floattype(hir_subject_type_t t);
int HIR_is_signtype(hir_subject_type_t t);
int HIR_is_globtype(hir_subject_type_t t);
int HIR_is_tmptype(hir_subject_type_t t);
int HIR_writeop(hir_operation_t op);
int HIR_isterm(hir_operation_t op);
int HIR_issyst(hir_operation_t op);
hir_operation_t HIR_convop(hir_subject_type_t t);
int HIR_similar_type(hir_subject_type_t a, hir_subject_type_t b);

#endif