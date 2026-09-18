#ifndef CODE_EXT_03_STATE_IMPL_INC_
#define CODE_EXT_03_STATE_IMPL_INC_ 0

#include "mutable.inc"

container state_impl03 implements mutable03 {
    i32 state;

    @[override] function value(ptr state_impl03 self) -> i32;
    @[override] function add(ptr state_impl03 self, i32 delta) -> i0;
}

#endif
