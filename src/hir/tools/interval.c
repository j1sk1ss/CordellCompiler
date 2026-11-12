#include <hir/interval.h>

value_interval_t HIR_INT_get_max_subject_value_interval(hir_subject_t* s) {
    if (!s || s->ptr) return (value_interval_t) { .found = 0 };
    switch (s->t) {
        case HIR_TMPVARI8:    case HIR_TMPVARU8:    case HIR_STKVARI8:    case HIR_STKVARU8:
        case HIR_TMPVARI16:   case HIR_TMPVARU16:   case HIR_STKVARI16:   case HIR_STKVARU16:
        case HIR_TMPVARI32:   case HIR_TMPVARU32:   case HIR_STKVARI32:   case HIR_STKVARU32:
        case HIR_TMPVARI64:   case HIR_TMPVARU64:   case HIR_STKVARI64:   case HIR_STKVARU64:
        case HIR_I8CONSTVAL:  case HIR_U8CONSTVAL:  case HIR_I16CONSTVAL: case HIR_U16CONSTVAL:
        case HIR_I32CONSTVAL: case HIR_U32CONSTVAL: case HIR_I64CONSTVAL: case HIR_U64CONSTVAL:
        case HIR_I8NUMBER:    case HIR_U8NUMBER:    case HIR_I16NUMBER:   case HIR_U16NUMBER:
        case HIR_I32NUMBER:   case HIR_U32NUMBER:   case HIR_I64NUMBER:   case HIR_U64NUMBER: {
            int bits = HIR_get_type_size(s->t) * 8;
            if (bits < 1 || bits > CONF_get_full_bytness() * 8) return (value_interval_t) { .found = 0 };
            return (value_interval_t) {
                .found = 1,
                .high  = ((big_int_t)1 << (bits - HIR_is_sign(s->t))) - 1,
                .low   = HIR_is_sign(s->t) ? -((big_int_t)1 << (bits - 1)) : 0
            };
        }
        default: return (value_interval_t) { .found = 0 };
    }
}

int HIR_INT_same_value_intervals(hir_subject_t* a, hir_subject_t* b) {
    value_interval_t av = HIR_INT_get_max_subject_value_interval(a);
    value_interval_t bv = HIR_INT_get_max_subject_value_interval(b);
    return av.found && bv.found && av.low == bv.low && av.high == bv.high;
}

int HIR_INT_suitable(hir_subject_t* container, hir_subject_t* data) {
    value_interval_t vv = HIR_INT_get_max_subject_value_interval(container);
    value_interval_t cv = HIR_INT_get_max_subject_value_interval(data);
    return vv.found && cv.found && (HIR_is_sign(container->t) == HIR_is_sign(data->t) || (vv.low >= cv.low && vv.high <= cv.high));
}
