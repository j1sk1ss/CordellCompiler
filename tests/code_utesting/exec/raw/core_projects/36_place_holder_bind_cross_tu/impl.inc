#ifndef CODE_EXT_36_IMPL_INC_
#define CODE_EXT_36_IMPL_INC_ 0
#include "iface.inc"
container impl36 implements value36 {
    i32 value_field;
    @[override] function value(ptr impl36 self) -> i32;
}
function make_impl36() -> ptr value36;
#endif
