#include <lir/lir_types.h>

int LIR_is_global_hirtype(hir_subject_type_t t) {
    switch (t) {
        case HIR_GLBVARSTR:
        case HIR_GLBVARARR:
        case HIR_GLBVARF64:
        case HIR_GLBVARI64:
        case HIR_GLBVARU64:
        case HIR_GLBVARF32:
        case HIR_GLBVARI32:
        case HIR_GLBVARU32:
        case HIR_GLBVARI16:
        case HIR_GLBVARU16:
        case HIR_GLBVARI8:
        case HIR_GLBVARU8:  return 1;
        default: return 0;
    }
}

int LIR_get_hirtype_size(hir_subject_type_t t) {
    switch (t) {
        case HIR_TMPVARF64:
        case HIR_TMPVARI64:
        case HIR_TMPVARU64:
        case HIR_STKVARF64:
        case HIR_STKVARI64:
        case HIR_STKVARU64:
        case HIR_GLBVARF64:
        case HIR_GLBVARI64:
        case HIR_GLBVARU64: return DEFAULT_TYPE_SIZE;
        case HIR_TMPVARF32:
        case HIR_TMPVARI32:
        case HIR_TMPVARU32:
        case HIR_STKVARF32:
        case HIR_STKVARI32:
        case HIR_STKVARU32:
        case HIR_GLBVARF32:
        case HIR_GLBVARI32:
        case HIR_GLBVARU32: return 4;
        case HIR_TMPVARI16:
        case HIR_TMPVARU16:
        case HIR_STKVARI16:
        case HIR_STKVARU16:
        case HIR_GLBVARI16:
        case HIR_GLBVARU16: return 2;
        case HIR_TMPVARI8:
        case HIR_TMPVARU8:
        case HIR_STKVARI8:
        case HIR_STKVARU8:
        case HIR_GLBVARI8:
        case HIR_GLBVARU8:  return 1;
        default: return DEFAULT_TYPE_SIZE;
    }
}
