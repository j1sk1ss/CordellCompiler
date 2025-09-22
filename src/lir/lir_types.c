#include <lir/lir_types.h>

int LIR_is_global_hirtype(hir_subject_type_t t) {
    switch (t) {
        case GLBVARSTR:
        case GLBVARARR:
        case GLBVARF64:
        case GLBVARI64:
        case GLBVARU64:
        case GLBVARF32:
        case GLBVARI32:
        case GLBVARU32:
        case GLBVARI16:
        case GLBVARU16:
        case GLBVARI8:
        case GLBVARU8:  return 1;
        default:        return 0;
    }
}

int LIR_get_hirtype_size(hir_subject_type_t t) {
    switch (t) {
        case TMPVARF64:
        case TMPVARI64:
        case TMPVARU64:
        case STKVARF64:
        case STKVARI64:
        case STKVARU64:
        case GLBVARF64:
        case GLBVARI64:
        case GLBVARU64: return DEFAULT_TYPE_SIZE;
        case TMPVARF32:
        case TMPVARI32:
        case TMPVARU32:
        case STKVARF32:
        case STKVARI32:
        case STKVARU32:
        case GLBVARF32:
        case GLBVARI32:
        case GLBVARU32: return 4;
        case TMPVARI16:
        case TMPVARU16:
        case STKVARI16:
        case STKVARU16:
        case GLBVARI16:
        case GLBVARU16: return 2;
        case TMPVARI8:
        case TMPVARU8:
        case STKVARI8:
        case STKVARU8:
        case GLBVARI8:
        case GLBVARU8:  return 1;
        default:        return DEFAULT_TYPE_SIZE;
    }
}
