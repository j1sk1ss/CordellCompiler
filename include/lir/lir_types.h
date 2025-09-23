#ifndef LIR_TYPES_H_
#define LIR_TYPES_H_

#include <hir/hir_types.h>

#define DEFAULT_TYPE_SIZE 8

typedef enum {
    LIR_REGISTER,
    LIR_STVARIABLE,
    LIR_GLVARIABLE,
    LIR_CONSTVAL,
    LIR_NUMBER,
    LIR_LABEL,
    LIR_RAWASM,
    LIR_MEMORY,
    LIR_FNAME
} lir_subject_type_t;

int LIR_get_asttype_size(token_type_t t);
int LIR_get_hirtype_size(hir_subject_type_t t);
int LIR_is_global_hirtype(hir_subject_type_t t);

#endif