#ifndef CODE_EXT_22_IMPL_INC_
#define CODE_EXT_22_IMPL_INC_ 0
#include "iface.inc"
container impl22 implements value22 {
    i32 base;
    @[self] function helper(ptr impl22 self) -> i32;
    @[override] function value(ptr impl22 self) -> i32;
}
#endif
