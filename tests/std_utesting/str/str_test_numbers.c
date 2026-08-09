#include <std/str.h>
#include "../../misc/testing.h"

static int _near(double a, double b) {
    double diff = a - b;
    return diff < 0.000001 && diff > -0.000001;
}

static int _check_number(const char* raw, const char* expected, int is_float_expected) {
    string_t* s = create_string(raw);
    int is_float = 0;
    string_t* converted = s->from_number(s, &is_float);
    assert(converted, "Number conversion failed!");
    assert(is_float == is_float_expected, "Float flag is incorrect!");
    assert(converted->requals(converted, expected), "Converted number is incorrect!");
    destroy_string(converted);
    destroy_string(s);
    return 1;
}

int main() {
    mm_init();
    string_t* dec = create_string(" \t-123tail");
    string_t* dbl = create_string(" -1.25e+2x");
    string_t* dbl_neg_exp = create_string("+1.5e-1");

    assert(dec->to_llong(dec) == -123, "Signed long long parsing failed!");
    assert(_near(dbl->to_double(dbl), -125.0), "Positive exponent double parsing failed!");
    assert(_near(dbl_neg_exp->to_double(dbl_neg_exp), 0.15), "Negative exponent double parsing failed!");
    string_t* hex = create_string("7f");
    assert(hex->to_ullong(hex, 16) == 127, "Hex parsing failed!");
    destroy_string(hex);
    hex = create_string("7F");
    assert(hex->to_ullong(hex, 16) == 127, "Upper hex parsing failed!");
    destroy_string(hex);
    hex = create_string("12z");
    assert(hex->to_ullong(hex, 10) == 12, "Invalid digit stop failed!");
    destroy_string(hex);

    assert(_check_number("0x10", "16", 0), "Hex number conversion failed!");
    assert(_check_number("0b101", "5", 0), "Binary number conversion failed!");
    assert(_check_number("077", "63", 0), "Octal number conversion failed!");
    assert(_check_number("-42", "-42", 0), "Decimal number conversion failed!");

    string_t* float_src = create_string("1.5");
    int is_float = 0;
    string_t* float_bits = float_src->from_number(float_src, &is_float);
    assert(is_float == 1 && float_bits, "Float conversion failed!");
    destroy_string(float_bits);
    destroy_string(float_src);

    destroy_string(dbl_neg_exp);
    destroy_string(dbl);
    destroy_string(dec);
    return 0;
}
