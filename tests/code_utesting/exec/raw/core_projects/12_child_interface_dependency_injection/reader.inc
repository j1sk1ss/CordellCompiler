#ifndef CODE_EXT_12_READER_INC_
#define CODE_EXT_12_READER_INC_ 0

interface reader12 {
    @[self] @[abstract]
    function value(ptr reader12 self) -> i32;
}

function consume_root12(ptr reader12 item) -> i32;

#endif
