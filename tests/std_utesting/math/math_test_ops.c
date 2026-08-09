#include <std/math.h>
#include "../../misc/testing.h"

static int _near(double a, double b) {
    double diff = a - b;
    return diff < 0.000001 && diff > -0.000001;
}

int main() {
    assert(ALIGN(13, 8) == 16, "ALIGN failed!");
    assert(MIN(3, 8) == 3, "MIN failed!");
    assert(MAX(3, 8) == 8, "MAX failed!");
    assert(ABS(-9) == 9 && ABS(9) == 9, "ABS failed!");

    assert(_near(mth_pow(2.0, 10), 1024.0), "Positive power failed!");
    assert(_near(mth_pow(2.0, -2), 0.25), "Negative power failed!");
    assert(_near(mth_pow(7.0, 0), 1.0), "Zero power failed!");

    assert(mth_sqrt(-1) == 0, "Negative sqrt failed!");
    assert(mth_sqrt(0) == 0, "Zero sqrt failed!");
    assert(mth_sqrt(1) == 1, "One sqrt failed!");
    assert(mth_sqrt(49) == 7, "Exact sqrt failed!");
    assert(mth_sqrt(50) == 7, "Floor sqrt failed!");

    assert(mth_log2(-5) == 0, "Negative log2 failed!");
    assert(mth_log2(0) == 0, "Zero log2 failed!");
    assert(mth_log2(1) == 0, "One log2 failed!");
    assert(mth_log2(16) == 4, "Exact log2 failed!");
    assert(mth_log2(31) == 4, "Floor log2 failed!");
    return 0;
}
