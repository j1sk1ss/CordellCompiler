#include "repository.inc"

function repository11::total(ptr repository11 self, ptr scorer11 strategy) -> i32 {
    i32 i = 0;
    i32 result = 0;

    while i < 3; {
        result += strategy.score(ref self.items[i]);
        i += 1;
    }

    return result;
}
