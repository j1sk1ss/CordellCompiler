#include <str.h>

void* str_memcpy(void* destination, const void* source, size_t num) {
    unsigned int num_dwords = num / 4;
    unsigned int num_bytes  = num % 4;
    unsigned int* dest32 = (unsigned int*)destination;
    unsigned int* src32  = (unsigned int*)source;
    unsigned char* dest8 = ((unsigned char*)destination) + num_dwords * 4;
    unsigned char* src8  = ((unsigned char*)source) + num_dwords * 4;
    unsigned int i = 0;

    for (i = 0; i < num_dwords; i++) dest32[i] = src32[i];
    for (i = 0; i < num_bytes; i++) dest8[i] = src8[i];
    return destination;
}

void* str_memset(void* pointer, unsigned char value, size_t num) {
    unsigned int num_dwords = num / 4;
    unsigned int num_bytes  = num % 4;
    unsigned int* dest32 = (unsigned int*)pointer;
    unsigned char* dest8 = ((unsigned char*)pointer) + num_dwords * 4;
    unsigned char val8   = (unsigned char)value;
    unsigned int val32   = value | (value << 8) | (value << 16) | (value << 24);
    unsigned int i = 0;

    for (i = 0; i < num_dwords; i++) dest32[i] = val32;
    for (i = 0; i < num_bytes; i++) dest8[i] = val8;
    return pointer;
}

int str_memcmp(const void* firstPointer, const void* secondPointer, size_t num) {
    const unsigned char* u8Ptr1 = (const unsigned char *)firstPointer;
    const unsigned char* u8Ptr2 = (const unsigned char *)secondPointer;
    for (unsigned short i = 0; i < num; i++)
        if (u8Ptr1[i] != u8Ptr2[i])
            return 1;

    return 0;
}

char* str_strncpy(char* dst, const char* src, int n) {
	int	i = 0;
	while (i < n && src[i]) {
		dst[i] = src[i];
		i++;
	}

	while (i < n) {
		dst[i] = 0;
		i++;
	}

	return dst;
}

int str_strcmp(const char* f, const char* s) {
    if (!f || !s) return -1;
    while (*f && *s && *f == *s) {
        ++f;
        ++s;
    }

    return (unsigned char)(*f) - (unsigned char)(*s);
}

int str_strncmp(const char* str1, const char* str2, size_t n) {
    for (size_t i = 0; i < n; ++i) {
        if (str1[i] != str2[i] || !str1[i] || !str2[i]) 
            return (unsigned char)str1[i] - (unsigned char)str2[i];
    }

    return 0;
}

int str_atoi(const char *str) {
    int neg = 1;
    long long num = 0;
    size_t i = 0;

    while (*str == ' ') str++;
    if (*str == '-' || *str == '+') {
        neg = *str == '-' ? -1 : 1;
        str++;
    }

	while (*str >= '0' && *str <= '9' && *str) {
		num = num * 10 + (str[i] - 48);
        if (neg == 1 && num > INT_MAX) return INT_MAX;
        if (neg == -1 && -num < INT_MIN) return INT_MIN;
		str++;
	}
    
	return (num * neg);
}

unsigned int str_strlen(const char* str) {
    unsigned int len = 0;
    while (*str) {
        ++len;
        ++str;
    }

    return len;
}

char* str_strcpy(char* dst, const char* src) {
    if (str_strlen(src) <= 0) return NULL;

	int	i = 0;
	while (src[i]) {
		dst[i] = src[i];
		i++;
	}

	dst[i] = 0;
	return (dst);
}

const char* str_strchr(const char* str, char chr) {
    if (!str) return NULL;
    while (*str) {
        if (*str == chr) return str;
        ++str;
    }

    return NULL;
}

char* str_strcat(char* dest, const char* src) {
    str_strcpy(dest + str_strlen(dest), src);
    return dest;
}

int is_number(char* s) {
    while (*s) {
        if (!str_isdigit(*s)) return 0;
        s++;
    }

    return 1;
}

int str_isdigit(int c) {
    return (c >= '0' && c <= '9');
}

int str_isspace(int c) {
    return (c == ' ' || c == '\n' || c == '\r' || c == '\t' || c == '\v' || c == '\b' || c == '\0');
}

unsigned long long str_strtoull(const char* str, int l, int base) {
    unsigned long long result = 0;

    char c;
    while ((c = *str++) && l-- > 0) {
        int digit = 0;
        if (c >= '0' && c <= '9') digit = c - '0';
        else if (c >= 'a' && c <= 'f') digit = c - 'a' + 10;
        else if (c >= 'A' && c <= 'F') digit = c - 'A' + 10;
        else break;

        if (digit >= base) break;
        result = result * base + digit;
    }

    return result;
}
