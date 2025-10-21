#include <std/math.h>

double mth_pow(double base, int exp) {
    double result = 1.0;
    int negative = 0;

    if (exp < 0) {
        negative = 1;
        exp = -exp;
    }

    while (exp) {
        if (exp & 1) result *= base;
        base *= base;
        exp >>= 1;
    }

    return negative ? 1.0 / result : result;
}
