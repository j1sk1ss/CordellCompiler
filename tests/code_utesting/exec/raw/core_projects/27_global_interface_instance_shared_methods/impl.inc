#ifndef CODE_EXT_27_IMPL_INC_
#define CODE_EXT_27_IMPL_INC_ 0
#include "counter.inc"
container impl27 implements counter27 {
    i32 state;
    @[override] function add(ptr impl27 self, i32 delta) -> i0;
    @[override] function value(ptr impl27 self) -> i32;
}
#endif
