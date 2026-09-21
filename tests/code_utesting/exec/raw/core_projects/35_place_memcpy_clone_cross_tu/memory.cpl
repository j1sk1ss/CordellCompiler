#include "memory.inc"
function memcpy35(ptr i0 dst, ptr i0 src, i64 count) -> ptr i0 {
    ptr u8 d = dst as ptr u8;
    ptr u8 s = src as ptr u8;
    i64 i = 0;
    while i < count; {
        d[i] = s[i];
        i += 1;
    }
    return dst;
}
