#ifndef CODE_EXT_35_IMPL_INC_
#define CODE_EXT_35_IMPL_INC_ 0
#include "iface.inc"
container impl35 implements value35 {
    i32 value_field;
    @[override] function value(ptr impl35 self) -> i32;
}
#endif
