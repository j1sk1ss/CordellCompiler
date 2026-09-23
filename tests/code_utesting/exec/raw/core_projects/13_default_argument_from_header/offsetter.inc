#ifndef CODE_EXT_13_OFFSETTER_INC_
#define CODE_EXT_13_OFFSETTER_INC_ 0

container offsetter13 {
    i32 value;

    @[self]
    function add(ptr offsetter13 self, i32 delta = 5 as i32) -> i0;
}

function workflow13(ptr offsetter13 item) -> i32;

#endif
