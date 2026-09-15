#ifndef CODE_EXT_11_REPOSITORY_INC_
#define CODE_EXT_11_REPOSITORY_INC_ 0

#include "domain.inc"
#include "scorer.inc"

container repository11 {
    arr items[3, record11];

    @[self]
    function total(ptr repository11 self, ptr scorer11 strategy) -> i32;
}

#endif
