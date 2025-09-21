#ifndef LIR_TYPES_H_
#define LIR_TYPES_H_

typedef enum {
    REGISTER,
    STVARIABLE,
    GLVARIABLE,
    CONSTVAL,
    LABEL,
    RAWASM,
} lir_subject_type_t;

#endif