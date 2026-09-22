#ifndef CODE_EXT_02_OPERATION_INC_
#define CODE_EXT_02_OPERATION_INC_ 0

interface operation02 {
    @[self] @[abstract]
    function apply(ptr operation02 self, i32 value) -> i32;
}

#endif
