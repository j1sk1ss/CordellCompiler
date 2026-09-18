#ifndef CODE_EXT_18_IMPL_INC_
#define CODE_EXT_18_IMPL_INC_ 0
#include "iface.inc"
container impl18 implements value18 {
    i32 state;
    @[override] function value(ptr impl18 self) -> i32;
    @[override] function add_other(ptr impl18 self, ptr value18 other) -> i32;
}
#endif
