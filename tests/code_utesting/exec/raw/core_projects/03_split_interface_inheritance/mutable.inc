#ifndef CODE_EXT_03_MUTABLE_INC_
#define CODE_EXT_03_MUTABLE_INC_ 0

#include "root.inc"

interface mutable03 implements root03 {
    @[self] @[abstract]
    function add(ptr mutable03 self, i32 delta) -> i0;
}

#endif
