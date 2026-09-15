#ifndef CODE_EXT_32_IMPL_INC_
#define CODE_EXT_32_IMPL_INC_ 0
#include "child.inc"
container impl32::mutable32 {
    i32 state;
    @[override] function value(ptr impl32 self) -> i32;
    @[override] function add(ptr impl32 self, i32 delta) -> i0;
}
function workflow32(ptr mutable32 item) -> i32;
#endif
