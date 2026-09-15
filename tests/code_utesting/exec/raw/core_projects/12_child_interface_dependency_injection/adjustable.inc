#ifndef CODE_EXT_12_ADJUSTABLE_INC_
#define CODE_EXT_12_ADJUSTABLE_INC_ 0

#include "reader.inc"

interface adjustable12::reader12 {
    @[self] @[abstract]
    function set_offset(ptr adjustable12 self, i32 offset) -> i0;
}

#endif
