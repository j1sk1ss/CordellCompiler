#ifndef CODE_EXT_14_HOLDER_INC_
#define CODE_EXT_14_HOLDER_INC_ 0
#include "iface.inc"
container holder14 {
    ptr value14 item;
    @[self] function read(ptr holder14 self) -> i32;
}
function bind14(ptr holder14 holder, ptr value14 item) -> i0;
#endif
