#ifndef CODE_EXT_23_IMPL_INC_
#define CODE_EXT_23_IMPL_INC_ 0
#include "iface.inc"
container impl23::value23 { ptr i32 source; @[override] function value(ptr impl23 self) -> i32; }
function attach23(ptr impl23 item, ptr i32 source) -> i0;
#endif
