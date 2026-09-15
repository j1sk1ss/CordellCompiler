#ifndef CODE_EXT_16_IMPL_INC_
#define CODE_EXT_16_IMPL_INC_ 0
#include "iface.inc"
container impl16::sum16 {
    arr values[3, i32];
    @[override] function sum(ptr impl16 self) -> i32;
}
function call16(ptr sum16 item) -> i32;
#endif
