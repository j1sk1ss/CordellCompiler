#ifndef CODE_EXT_18_IFACE_INC_
#define CODE_EXT_18_IFACE_INC_ 0
interface value18 {
    @[self] @[abstract] function value(ptr value18 self) -> i32;
    @[self] @[abstract] function add_other(ptr value18 self, ptr value18 other) -> i32;
}
#endif
