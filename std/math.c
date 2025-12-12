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

long mth_sqrt(long x) {
    if (x < 0) return 0;
    if (x == 0 || x == 1) return x;
    long left = 0, right = x, ans = 0;
    while (left <= right) {
        long mid = left + (right - left) / 2;
        long mid_sq = mid * mid;
        if (mid_sq == x) return mid;
        if (mid_sq < x) {
            left = mid + 1;
            ans = mid;
        } 
        else {
            right = mid - 1;
        }
    }
    
    return ans;
}

long mth_log2(long x) {
    if (x <= 0) return 0;
    long log = 0;
    while (x >>= 1) {
        log++;
    }

    return log;
}
