#include <std/mem.h>

void* str_memcpy(void* dst, const void* src, unsigned long n) {
    unsigned int num_dwords = n / 4;
    unsigned int num_bytes  = n % 4;
    unsigned int* dest32 = (unsigned int*)dst;
    unsigned int* src32  = (unsigned int*)src;
    unsigned char* dest8 = ((unsigned char*)dst) + num_dwords * 4;
    unsigned char* src8  = ((unsigned char*)src) + num_dwords * 4;
    unsigned int i = 0;

    for (i = 0; i < num_dwords; i++) dest32[i] = src32[i];
    for (i = 0; i < num_bytes; i++) dest8[i] = src8[i];
    return dst;
}

void* str_memset(void* ptr, unsigned char v, unsigned long n) {
    unsigned int num_dwords = n / 4;
    unsigned int num_bytes  = n % 4;
    unsigned int* dest32 = (unsigned int*)ptr;
    unsigned char* dest8 = ((unsigned char*)ptr) + num_dwords * 4;
    unsigned char val8   = (unsigned char)v;
    unsigned int val32   = v | (v << 8) | (v << 16) | (v << 24);
    unsigned int i = 0;

    for (i = 0; i < num_dwords; i++) dest32[i] = val32;
    for (i = 0; i < num_bytes; i++) dest8[i] = val8;
    return ptr;
}

int str_memcmp(const void* dst, const void* src, unsigned long n) {
    for (unsigned short i = 0; i < n; i++) {
        if (((const unsigned char*)dst)[i] != ((const unsigned char*)src)[i]) {
            return 1;
        }
    }

    return 0;
}

unsigned long long str_dob2bits(double d) {
    unsigned long long bits = 0;
    str_memcpy(&bits, &d, sizeof(d));
    return bits;
}

double str_bits2dob(unsigned long long bits) {
    double d = 0.0;
    str_memcpy(&d, &bits, sizeof(d));
    return d;
}
