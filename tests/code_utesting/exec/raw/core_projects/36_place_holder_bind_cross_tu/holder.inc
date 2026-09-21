#ifndef CODE_EXT_36_HOLDER_INC_
#define CODE_EXT_36_HOLDER_INC_ 0
#include "iface.inc"
container holder36 {
    ptr value36 item;
    @[self] function read(ptr holder36 self) -> i32;
}
function bind36(ptr holder36 holder, ptr value36 item) -> i0;
#endif
