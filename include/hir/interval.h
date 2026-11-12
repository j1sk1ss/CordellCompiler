#ifndef HIR_INTERVAL_H_
#define HIR_INTERVAL_H_

#include <hir/hir.h>

typedef struct {
    big_int_t high;
    big_int_t low;
    int       found : 1;
} value_interval_t;

value_interval_t HIR_INT_get_max_subject_value_interval(hir_subject_t* s);
int              HIR_INT_same_value_intervals(hir_subject_t* a, hir_subject_t* b);
int              HIR_INT_suitable(hir_subject_t* container, hir_subject_t* data);

#endif
