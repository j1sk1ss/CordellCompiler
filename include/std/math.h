#ifndef MATH_H_
#define MATH_H_

#define ALIGN(x, b) ((x + b - 1) & ~(b - 1))
#define MIN(a,b)    (((a) < (b)) ? (a) : (b))
#define MAX(a,b)    (((a) > (b)) ? (a) : (b))
#define ABS(x)      ((x) < 0 ? -(x) : (x))

double mth_pow(double base, int exp);
long mth_log2(long x);
long mth_sqrt(long x);

#endif