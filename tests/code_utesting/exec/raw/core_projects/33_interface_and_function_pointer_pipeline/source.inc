#ifndef CODE_EXT_33_SOURCE_INC_
#define CODE_EXT_33_SOURCE_INC_ 0
interface source33 { @[self] @[abstract] function value(ptr source33 self) -> i32; }
container fixed33::source33 { i32 state; @[override] function value(ptr fixed33 self) -> i32; }
function add_two33(i32 value) -> i32;
function pipeline33(ptr source33 source, ptr fn(i32)i32 transform) -> i32;
#endif
