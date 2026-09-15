#ifndef CODE_EXT_02_MUL_OP_INC_
#define CODE_EXT_02_MUL_OP_INC_ 0

#include "operation.inc"

container mul_op02::operation02 {
    i32 factor;

    @[override]
    function apply(ptr mul_op02 self, i32 value) -> i32;
}

#endif
