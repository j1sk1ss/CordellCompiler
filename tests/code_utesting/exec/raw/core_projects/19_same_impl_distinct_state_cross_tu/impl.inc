#ifndef CODE_EXT_19_IMPL_INC_
#define CODE_EXT_19_IMPL_INC_ 0
#include "iface.inc"
container impl19::value19 { i32 state; @[override] function value(ptr impl19 self) -> i32; }
function sum19(ptr value19 a, ptr value19 b) -> i32;
#endif
