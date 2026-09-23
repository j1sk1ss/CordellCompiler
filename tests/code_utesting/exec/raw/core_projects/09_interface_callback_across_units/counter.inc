#ifndef CODE_EXT_09_COUNTER_INC_
#define CODE_EXT_09_COUNTER_INC_ 0

#include "sink.inc"

container counter09 implements sink09 {
    i32 value;

    @[override] function push(ptr counter09 self, i32 value) -> i0;
    @[override] function total(ptr counter09 self) -> i32;
}

#endif
