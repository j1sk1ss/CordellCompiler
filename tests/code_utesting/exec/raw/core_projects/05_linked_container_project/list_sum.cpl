#include "list.inc"

function list05::sum(ptr list05 self) -> i32 {
    ptr node05 current = self.head;
    i32 total = 0;

    while current; {
        total += current.value;
        current = current.next;
    }

    return total;
}
