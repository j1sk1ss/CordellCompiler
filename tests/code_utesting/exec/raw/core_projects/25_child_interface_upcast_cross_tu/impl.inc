#ifndef CODE_EXT_25_IMPL_INC_
#define CODE_EXT_25_IMPL_INC_ 0
#include "child.inc"
container impl25::child25 {
    @[override] function first(ptr impl25 self) -> i32;
    @[override] function second(ptr impl25 self) -> i32;
}
function consume_child25(ptr child25 item) -> i32;
#endif
