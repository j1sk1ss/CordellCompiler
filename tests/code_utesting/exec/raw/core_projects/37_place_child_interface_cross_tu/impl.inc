#ifndef CODE_EXT_37_IMPL_INC_
#define CODE_EXT_37_IMPL_INC_ 0
#include "child.inc"
container impl37 implements child37 {
    i32 state;
    @[override] function value(ptr impl37 self) -> i32;
    @[override] function add(ptr impl37 self, i32 delta) -> i0;
}
function make_child37() -> ptr child37;
#endif
