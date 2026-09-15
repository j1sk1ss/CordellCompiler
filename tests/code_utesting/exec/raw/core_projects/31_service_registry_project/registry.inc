#ifndef CODE_EXT_31_REGISTRY_INC_
#define CODE_EXT_31_REGISTRY_INC_ 0
#include "service.inc"
container registry31 {
    arr services[2, ptr service31];
    @[self] function bind(ptr registry31 self, ptr service31 a, ptr service31 b) -> i0;
    @[self] function total(ptr registry31 self) -> i32;
}
#endif
