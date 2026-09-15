#ifndef CODE_EXT_02_ADD_OP_INC_
#define CODE_EXT_02_ADD_OP_INC_ 0

#include "operation.inc"

container add_op02::operation02 {
    i32 bias;

    @[override]
    function apply(ptr add_op02 self, i32 value) -> i32;
}

#endif
