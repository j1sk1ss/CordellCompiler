#include <prep/token.h>
#include "../../misc/testing.h"

static int _check_value(token_type_t input_type, const char* input, token_type_t expected_type, const char* expected) {
    int allocated = mm_get_allocated();
    token_t* token = TKN_create_token(input_type, input, NULL);
    assert(token && token->body, "Token or body allocation failed!");
    if (token->t_type != expected_type || !token->body->requals(token->body, expected)) {
        fprintf(stderr, "Wrong token for '%s': type=%d, body='%s'\n", input, token->t_type, token->body->head);
        TKN_unload_token(token);
        return 1;
    }
    assert(TKN_unload_token(token), "Token cleanup failed!");
    assert(mm_get_allocated() == allocated, "Token creation leaked memory!");
    return 0;
}

static int _test_metadata() {
    string_t* filename = create_string("source.cpl");
    assert(filename, "Filename allocation failed!");
    file_position_t pos = { .line = 12, .column = 34, .file = filename };
    char text[] = "identifier";
    token_t* token = TKN_create_token(UNKNOWN_STRING_TOKEN, text, &pos);
    assert(token && token->body, "Identifier creation failed!");
    text[0] = 'X';
    pos.line = 99;
    pos.column = 88;
    pos.file = NULL;
    assert(token->body->requals(token->body, "identifier"), "Token retained caller's text buffer!");
    assert(token->finfo.line == 12 && token->finfo.column == 34 && token->finfo.file == filename, "Source position was not copied!");
    assert(!token->flags.ptr && !token->flags.ro && !token->flags.glob && !token->flags.ext && !token->flags.vla, "New token has nonzero flags!");
    assert(TKN_unload_token(token), "Identifier cleanup failed!");
    assert(filename->requals(filename, "source.cpl"), "Token cleanup damaged borrowed filename!");
    destroy_string(filename);

    token = TKN_create_token(EOF_TOKEN, NULL, NULL);
    assert(token && token->t_type == EOF_TOKEN && !token->body, "Bodyless EOF creation failed!");
    assert(!token->finfo.line && !token->finfo.column && !token->finfo.file, "Missing source position was not zeroed!");
    assert(TKN_unload_token(token), "Bodyless token cleanup failed!");
    assert(!TKN_unload_token(NULL), "NULL token cleanup unexpectedly succeeded!");
    return 0;
}

static int _test_values() {
    const struct {
        token_type_t input_type;
        const char* input;
        token_type_t expected_type;
        const char* expected;
    } cases[] = {
        {UNKNOWN_STRING_TOKEN, "", UNKNOWN_STRING_TOKEN, ""},
        {STRING_VALUE_TOKEN, "hello\nworld", STRING_VALUE_TOKEN, "hello\nworld"},
        {DELIMITER_TOKEN, ";", DELIMITER_TOKEN, ";"},
        {UNKNOWN_SIGN_TOKEN, "+=", UNKNOWN_CHAR_TOKEN, "+="},
        {CHAR_VALUE_TOKEN, "A", UNKNOWN_NUMERIC_TOKEN, "65"},
        {CHAR_VALUE_TOKEN, "\n", UNKNOWN_NUMERIC_TOKEN, "10"},
        {CHAR_VALUE_TOKEN, "\t", UNKNOWN_NUMERIC_TOKEN, "9"},
        {UNKNOWN_NUMERIC_TOKEN, "0", UNKNOWN_NUMERIC_TOKEN, "0"},
        {UNKNOWN_NUMERIC_TOKEN, "42", UNKNOWN_NUMERIC_TOKEN, "42"},
        {UNKNOWN_NUMERIC_TOKEN, "-42", UNKNOWN_NUMERIC_TOKEN, "-42"},
        {UNKNOWN_NUMERIC_TOKEN, "0x2a", UNKNOWN_NUMERIC_TOKEN, "42"},
        {UNKNOWN_NUMERIC_TOKEN, "0XFF", UNKNOWN_NUMERIC_TOKEN, "255"},
        {UNKNOWN_NUMERIC_TOKEN, "0b101010", UNKNOWN_NUMERIC_TOKEN, "42"},
        {UNKNOWN_NUMERIC_TOKEN, "0B11", UNKNOWN_NUMERIC_TOKEN, "3"},
        {UNKNOWN_NUMERIC_TOKEN, "077", UNKNOWN_NUMERIC_TOKEN, "63"},
        {UNKNOWN_NUMERIC_TOKEN, "1.5", UNKNOWN_FLOAT_NUMERIC_TOKEN, "4609434218613702656"},
        {UNKNOWN_NUMERIC_TOKEN, "42i8", UNKNOWN_I8NUMERIC_TOKEN, "42"},
        {UNKNOWN_NUMERIC_TOKEN, "42i16", UNKNOWN_I16NUMERIC_TOKEN, "42"},
        {UNKNOWN_NUMERIC_TOKEN, "-42i32", UNKNOWN_I32NUMERIC_TOKEN, "-42"},
        {UNKNOWN_NUMERIC_TOKEN, "42i64", UNKNOWN_I64NUMERIC_TOKEN, "42"},
        {UNKNOWN_NUMERIC_TOKEN, "42u8", UNKNOWN_U8NUMERIC_TOKEN, "42"},
        {UNKNOWN_NUMERIC_TOKEN, "42u16", UNKNOWN_U16NUMERIC_TOKEN, "42"},
        {UNKNOWN_NUMERIC_TOKEN, "42u32", UNKNOWN_U32NUMERIC_TOKEN, "42"},
        {UNKNOWN_NUMERIC_TOKEN, "18446744073709551615u64", UNKNOWN_U64NUMERIC_TOKEN, "18446744073709551615"},
        {UNKNOWN_NUMERIC_TOKEN, "0xffu16", UNKNOWN_U16NUMERIC_TOKEN, "255"},
        {UNKNOWN_NUMERIC_TOKEN, "0b11i8", UNKNOWN_I8NUMERIC_TOKEN, "3"},
        {UNKNOWN_NUMERIC_TOKEN, "077u32", UNKNOWN_U32NUMERIC_TOKEN, "63"},
    };
    for (unsigned long i = 0; i < sizeof(cases) / sizeof(cases[0]); i++) {
        assert(!_check_value(cases[i].input_type, cases[i].input, cases[i].expected_type, cases[i].expected), "Token value test failed!");
    }
    
    char long_number[67];
    str_memset(long_number, '0', 63);
    str_memcpy(long_number + 63, "u8", 3);
    assert(!_check_value(UNKNOWN_NUMERIC_TOKEN, long_number, UNKNOWN_U8NUMERIC_TOKEN, "0"), "Maximum typed numeric length failed!");
    str_memset(long_number, '0', 64);
    str_memcpy(long_number + 64, "u8", 3);
    int allocated = mm_get_allocated();
    assert(!TKN_create_token(UNKNOWN_NUMERIC_TOKEN, long_number, NULL), "Oversized typed numeric text accepted!");
    assert(mm_get_allocated() == allocated, "Rejected numeric token leaked memory!");
    return 0;
}

int main() {
    mm_init();
    assert(!_test_metadata(), "Token metadata test failed!");
    assert(!_test_values(), "Token values test failed!");
    assert(mm_get_allocated() == 0, "Token tests leaked memory!");
    return 0;
}
