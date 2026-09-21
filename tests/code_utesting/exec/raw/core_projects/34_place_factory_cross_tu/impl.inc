#ifndef CODE_EXT_34_IMPL_INC_
#define CODE_EXT_34_IMPL_INC_ 0
#include "iface.inc"
container impl34 implements value34 {
    i32 value_field;
    @[override] function value(ptr impl34 self) -> i32;
}
#endif
