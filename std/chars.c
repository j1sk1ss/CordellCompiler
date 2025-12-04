#include <std/chars.h>

int str_isdigit(int c) {
    return (c >= '0' && c <= '9');
}

int is_number(char* s) {
    while (*s) {
        if (!str_isdigit(*s)) return 0;
        s++;
    }

    return 1;
}

int str_isspace(int c) {
    return (c == ' ' || c == '\n' || c == '\r' || c == '\t' || c == '\v' || c == '\b' || c == '\0');
}

int str_islower(int c) {
    return (c >= 'a' && c <= 'z');
}

int str_isupper(int c) {
    return (c >= 'A' && c <= 'Z');
}

int str_isalpha(int c) {
    return str_islower(c) || str_isupper(c);
}

int str_isalnum(int c) {
    return str_isalpha(c) || str_isdigit(c);
}
