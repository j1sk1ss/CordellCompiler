#ifndef CODE_EXT_31_OFFSET_INC_
#define CODE_EXT_31_OFFSET_INC_ 0
#include "service.inc"
container offset31 implements service31 { i32 base; i32 offset; @[override] function value(ptr offset31 self) -> i32; }
#endif
