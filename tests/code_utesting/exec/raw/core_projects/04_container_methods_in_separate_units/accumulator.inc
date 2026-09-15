#ifndef CODE_EXT_04_ACCUMULATOR_INC_
#define CODE_EXT_04_ACCUMULATOR_INC_ 0

container accumulator04 {
    i32 value;

    @[self]
    function add(ptr accumulator04 self, i32 delta) -> i0;

    @[self]
    function multiply(ptr accumulator04 self, i32 factor) -> i0;
}

function pipeline04(ptr accumulator04 item) -> i32;

#endif
