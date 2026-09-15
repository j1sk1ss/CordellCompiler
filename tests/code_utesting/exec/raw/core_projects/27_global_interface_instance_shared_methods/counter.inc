#ifndef CODE_EXT_27_COUNTER_INC_
#define CODE_EXT_27_COUNTER_INC_ 0
interface counter27 {
    @[self] @[abstract] function add(ptr counter27 self, i32 delta) -> i0;
    @[self] @[abstract] function value(ptr counter27 self) -> i32;
}
function global_counter27() -> ptr counter27;
function workflow27() -> i32;
#endif
