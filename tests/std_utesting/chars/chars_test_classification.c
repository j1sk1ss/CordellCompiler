#include <std/chars.h>
#include "../../misc/testing.h"

int main() {
    assert(str_isdigit('0'), "Digit lower bound failed!");
    assert(str_isdigit('9'), "Digit upper bound failed!");
    assert(!str_isdigit('a'), "Non-digit accepted!");

    assert(str_isspace(' '), "Space wasn't recognized!");
    assert(str_isspace('\n'), "Newline wasn't recognized!");
    assert(str_isspace('\r'), "Carriage return wasn't recognized!");
    assert(str_isspace('\t'), "Tab wasn't recognized!");
    assert(str_isspace('\v'), "Vertical tab wasn't recognized!");
    assert(str_isspace('\b'), "Backspace wasn't recognized!");
    assert(str_isspace(0), "Zero byte wasn't recognized as whitespace!");
    assert(!str_isspace('x'), "Non-space accepted!");

    assert(str_islower('a') && str_islower('z'), "Lowercase bounds failed!");
    assert(!str_islower('A'), "Uppercase accepted as lowercase!");
    assert(str_isupper('A') && str_isupper('Z'), "Uppercase bounds failed!");
    assert(!str_isupper('a'), "Lowercase accepted as uppercase!");

    assert(str_isalpha('b') && str_isalpha('B'), "Alpha check failed!");
    assert(!str_isalpha('7'), "Digit accepted as alpha!");
    assert(str_isalnum('b') && str_isalnum('7'), "Alnum check failed!");
    assert(!str_isalnum('_'), "Symbol accepted as alnum!");
    return 0;
}
