#ifndef CODE_EXT_09_SINK_INC_
#define CODE_EXT_09_SINK_INC_ 0

interface sink09 {
    @[self] @[abstract]
    function push(ptr sink09 self, i32 value) -> i0;

    @[self] @[abstract]
    function total(ptr sink09 self) -> i32;
}

#endif
