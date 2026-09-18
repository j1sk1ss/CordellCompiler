#ifndef CODE_EXT_11_SCORER_INC_
#define CODE_EXT_11_SCORER_INC_ 0

#include "domain.inc"

interface scorer11 {
    @[self] @[abstract]
    function score(ptr scorer11 self, ptr record11 item) -> i32;
}

container weight_scorer11 implements scorer11 {
    @[override]
    function score(ptr weight_scorer11 self, ptr record11 item) -> i32;
}

#endif
