#include "list.inc"

start() {
    node05 a;
    node05 b;
    node05 c;
    list05 items;

    a.value = 10;
    b.value = 14;
    c.value = 18;

    items.init();
    items.push_back(ref a);
    items.push_back(ref b);
    items.push_back(ref c);

    exit items.sum() as u8;
}
