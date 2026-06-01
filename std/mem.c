#include <std/mem.h>

void* str_memcpy(void* dst, const void* src, unsigned long n) {
    unsigned int num_dwords  = n / 4;
    unsigned int num_bytes   = n % 4;
    unsigned int* dest_dword = (unsigned int*)dst;
    unsigned int* src_dword  = (unsigned int*)src;
    unsigned char* dest_byte = ((unsigned char*)dst) + num_dwords * 4;
    unsigned char* src_byte  = ((unsigned char*)src) + num_dwords * 4;

    unsigned int i = 0;
    for (i = 0; i < num_dwords; i++) dest_dword[i] = src_dword[i];
    for (i = 0; i < num_bytes; i++) dest_byte[i] = src_byte[i];
    return dst;
}

int str_memcmp(void* dst, const void* src, unsigned long n) {
    unsigned char *s1 = (unsigned char*)dst;
    const unsigned char *s2 = (const unsigned char*)src;
    while (n-- > 0) if (*s1++ != *s2++) return 0;
    return 1;
}

void* str_memset(void* ptr, unsigned char v, unsigned long n) {
    unsigned int num_dwords  = n / 4;
    unsigned int num_bytes   = n % 4;
    unsigned int* dest_dword = (unsigned int*)ptr;
    unsigned char* dest_byte = ((unsigned char*)ptr) + num_dwords * 4;
    unsigned int val_dword   = v | (v << 8) | (v << 16) | (v << 24);
    unsigned char val_byte   = (unsigned char)v;

    unsigned int i = 0;
    for (i = 0; i < num_dwords; i++) dest_dword[i] = val_dword;
    for (i = 0; i < num_bytes; i++) dest_byte[i] = val_byte;
    return ptr;
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
