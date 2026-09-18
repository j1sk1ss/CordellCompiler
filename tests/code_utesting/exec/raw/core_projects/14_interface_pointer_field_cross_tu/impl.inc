#ifndef CODE_EXT_14_IMPL_INC_
#define CODE_EXT_14_IMPL_INC_ 0
#include "iface.inc"
container impl14 implements value14 {
    i32 state;
    @[override] function value(ptr impl14 self) -> i32;
}
#endif
