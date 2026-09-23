#ifndef CODE_EXT_31_FIXED_INC_
#define CODE_EXT_31_FIXED_INC_ 0
#include "service.inc"
container fixed31 implements service31 { i32 state; @[override] function value(ptr fixed31 self) -> i32; }
#endif
