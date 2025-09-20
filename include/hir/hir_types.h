#ifndef HIR_TYPES_H_
#define HIR_TYPES_H_

#include <hir/hir.h>

typedef enum {
    REGISTER,

    TMPVARSTR,
    TMPVARARR,
    TMPVARF64,
    TMPVARU64,
    TMPVARI64,
    TMPVARF32,
    TMPVARU32,
    TMPVARI32,
    TMPVARU16,
    TMPVARI16,
    TMPVARU8,
    TMPVARI8,

    GLBVARSTR,
    GLBVARARR,
    GLBVARF64,
    GLBVARU64,
    GLBVARI64,
    GLBVARF32,
    GLBVARU32,
    GLBVARI32,
    GLBVARU16,
    GLBVARI16,
    GLBVARU8,
    GLBVARI8,

    STKVARSTR,
    STKVARARR,
    STKVARF64,
    STKVARU64,
    STKVARI64,
    STKVARF32,
    STKVARU32,
    STKVARI32,
    STKVARU16,
    STKVARI16,
    STKVARU8,
    STKVARI8,

    CONSTVAL,

    LABEL,
    RAWASM,
    STRING,
} hir_subject_type_t;

int HIR_get_type_size(hir_subject_type_t t);
hir_subject_type_t HIR_promote_types(hir_subject_type_t a, hir_subject_type_t b);
hir_subject_type_t HIR_get_tmptype_tkn(token_t* token);
hir_subject_type_t _get_glbtype(int bitness, int isfloat, int issigned);
hir_subject_type_t HIR_get_stktype(token_t* token);

#endif