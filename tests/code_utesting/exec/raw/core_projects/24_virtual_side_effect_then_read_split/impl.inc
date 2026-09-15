#ifndef CODE_EXT_24_IMPL_INC_
#define CODE_EXT_24_IMPL_INC_ 0
#include "iface.inc"
container impl24::counter24 {
    i32 current;
    @[override] function bump(ptr impl24 self, i32 delta) -> i0;
    @[override] function value(ptr impl24 self) -> i32;
}
function workflow24(ptr counter24 item) -> i32;
#endif
