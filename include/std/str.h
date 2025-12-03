#ifndef STR_H_
#define STR_H_

#include <stddef.h>
#include <limits.h>
#include <std/logg.h>
#include <std/math.h>

/* Memory special functions */
void* str_memcpy(void* destination, const void* source, size_t num);
void* str_memset(void* pointer, unsigned char value, size_t num);
int str_memcmp(const void* firstPointer, const void* secondPointer, size_t num);
unsigned long long str_dob2bits(double d);
double str_bits2dob(unsigned long long bits);

/* String special functions */
char* str_strncpy(char* dst, const char* src, int n);
int str_strcmp(const char* f, const char* s);
int str_strncmp(const char* str1, const char* str2, size_t n);
int str_atoi(const char *str);
unsigned int str_strlen(const char* str);
char* str_strcpy(char* dst, const char* src);
char* str_strcat(char* dest, const char* src);
const char* str_strchr(const char* str, char chr);
unsigned long long str_strtoull(const char* str, int l, int base);
double str_strtod(const char* s, int l);
int write_value(const char* src, int src_size, char* dst, int dst_size);

/* ctype special functions */
int is_number(char* s);
int str_isdigit(int c);
int str_isspace(int c);
int str_islower(int c);
int str_isupper(int c);
int str_isdigit(int c);
int str_isalpha(int c);
int str_isalnum(int c);

#endif