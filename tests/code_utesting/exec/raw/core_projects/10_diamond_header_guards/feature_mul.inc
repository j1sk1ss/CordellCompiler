#ifndef CODE_EXT_10_FEATURE_MUL_INC_
#define CODE_EXT_10_FEATURE_MUL_INC_ 0

#include "transform.inc"

container mul_transform10 implements transform10 {
    i32 factor;
    @[override] function apply(ptr mul_transform10 self, i32 value) -> i32;
}

#endif
