#ifndef CODE_EXT_26_IFACE_INC_
#define CODE_EXT_26_IFACE_INC_ 0
interface value26 { @[self] @[abstract] function value(ptr value26 self) -> i32; }
function acquire26(i32 which) -> ptr value26;
function consume26(i32 which) -> i32;
#endif
