#ifndef STR_H_
#define STR_H_

#include <limits.h>
#include <std/mm.h>
#include <std/mem.h>
#include <std/logg.h>
#include <std/math.h>
#include <std/chars.h>

#define str_self struct string*
typedef struct string {
    /* fields */
    char*          head;
    char*          body;
    unsigned int   size;
    unsigned long  hash;

    /* functions */
    int                (*hmove)(str_self, unsigned int);
    int                (*rhead)(str_self);
    struct string*     (*copy)(str_self);
    unsigned int       (*len)(str_self);
    int                (*equals)(str_self, struct string*);
    int                (*requals)(str_self, struct string*);
    int                (*cat)(str_self, struct string*);
    struct string*     (*fchar)(str_self, char);
    double             (*to_double)(str_self);
    long long          (*to_llong)(str_self);
    unsigned long long (*to_ullong)(str_self, int);
    struct string*     (*from_number)(str_self);
} string_t;

string_t* create_string(char* s);
string_t* create_string_from_char(char c);
int destroy_string(string_t* s);

char* str_strncpy(char* dst, const char* src, int n);
int str_atoi(const char *str);
unsigned int str_strlen(const char* str);
char* str_strcpy(char* dst, const char* src);
char* str_strcat(char* dest, const char* src);
const char* str_strchr(const char* str, char chr);

#endif