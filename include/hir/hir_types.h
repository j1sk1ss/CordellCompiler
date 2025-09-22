#ifndef HIR_TYPES_H_
#define HIR_TYPES_H_

#include <hir/hir.h>

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
    HIR_STKVARI8,
    HIR_CONSTVAL,
    HIR_NUMBER,
    HIR_LABEL,
    HIR_RAWASM,
    HIR_STRING,
    HIR_FNAME,
} hir_subject_type_t;

int HIR_get_type_size(hir_subject_type_t t);
hir_subject_type_t HIR_promote_types(hir_subject_type_t a, hir_subject_type_t b);
hir_subject_type_t HIR_get_tmptype_tkn(token_t* token);
hir_subject_type_t _get_glbtype(int bitness, int isfloat, int issigned);
hir_subject_type_t HIR_get_stktype(token_t* token);
hir_subject_type_t HIR_get_tmp_type(hir_subject_type_t t);

#endif