#ifndef CODE_EXT_08_VALUE_SOURCE_INC_
#define CODE_EXT_08_VALUE_SOURCE_INC_ 0

interface value_source08 {
    @[self] @[abstract]
    function value(ptr value_source08 self) -> i32;
}

function acquire08(i32 value) -> ptr value_source08;
function consume08(i32 value) -> i32;

#endif
