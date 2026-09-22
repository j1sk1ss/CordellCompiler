#ifndef CODE_EXT_12_READER_IMPL_INC_
#define CODE_EXT_12_READER_IMPL_INC_ 0

#include "adjustable.inc"

container reader_impl12 implements adjustable12 {
    i32 base;
    i32 offset;

    @[override] function value(ptr reader_impl12 self) -> i32;
    @[override] function set_offset(ptr reader_impl12 self, i32 offset) -> i0;
}

#endif
