#include "impl.inc"
#include "memory.inc"
glob arr SRC35[64, u8];
glob arr DST35[64, u8];
function clone35() -> ptr value35 {
    ptr impl35 src = place(ref SRC35, impl35);
    src.value_field = 42;
    memcpy35(ref DST35, ref SRC35, sizeof(impl35));
    ptr impl35 dst = (ref DST35) as ptr impl35;
    return dst;
}
