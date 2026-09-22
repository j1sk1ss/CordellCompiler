#include "fixed.inc"
#include "offset.inc"
#include "registry.inc"
start() {
    fixed31 a; offset31 b; registry31 registry;
    a.state = 19; b.base = 20; b.offset = 3;
    registry.bind(ref a, ref b);
    exit registry.total() as u8;
}
