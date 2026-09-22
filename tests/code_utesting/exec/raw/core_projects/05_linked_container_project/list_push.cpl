#include "list.inc"

function list05::push_back(ptr list05 self, ptr node05 item) -> i0 {
    item.next = 0 as ptr node05;

    if self.tail; self.tail.next = item;
    else self.head = item;

    self.tail = item;
}
