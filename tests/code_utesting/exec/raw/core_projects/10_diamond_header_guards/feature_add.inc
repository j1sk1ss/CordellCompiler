#ifndef CODE_EXT_10_FEATURE_ADD_INC_
#define CODE_EXT_10_FEATURE_ADD_INC_ 0

#include "transform.inc"

container add_transform10 implements transform10 {
    i32 bias;
    @[override] function apply(ptr add_transform10 self, i32 value) -> i32;
}

#endif
