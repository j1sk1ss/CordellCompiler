#include <std/mem.h>
#include "../../misc/testing.h"

static int _near(double a, double b) {
    double diff = a - b;
    return diff < 0.000001 && diff > -0.000001;
}

int main() {
    unsigned char src[] = { 1, 2, 3, 4, 5, 6, 7 };
    unsigned char dst[7] = { 0 };
    unsigned char fill[9] = { 0 };

    assert(str_memcpy(dst, src, sizeof(src)) == dst, "memcpy return value failed!");
    assert(str_memcmp(dst, src, sizeof(src)), "memcpy content failed!");
    dst[3] = 99;
    assert(!str_memcmp(dst, src, sizeof(src)), "memcmp mismatch failed!");
    assert(str_memset(fill, 0xAB, sizeof(fill)) == fill, "memset return value failed!");
    for (int i = 0; i < (int)sizeof(fill); i++) {
        assert(fill[i] == 0xAB, "memset content failed!");
    }

    unsigned long long bits = str_dob2bits(3.5);
    assert(_near(str_bits2dob(bits), 3.5), "Double bit conversion failed!");
    return 0;
}
