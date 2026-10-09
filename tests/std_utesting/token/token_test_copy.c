#include <prep/token.h>
#include "../../misc/testing.h"

static int _test_copy() {
    string_t* filename = create_string("copy.cpl");
    assert(filename, "Filename allocation failed!");
    file_position_t pos = { .line = 7, .column = 11, .file = filename };
    token_t* source = TKN_create_token(UNKNOWN_STRING_TOKEN, "name", &pos);
    assert(source && source->body, "Source token creation failed!");
    source->flags = (basic_object_info_t){ .ptr = 2, .ro = 1, .glob = 1, .ext = 1, .vla = 1 };
    token_t* copy = TKN_copy_token(source);
    assert(copy && copy != source, "Copy did not allocate a distinct token!");
    assert(copy->body && copy->body != source->body && copy->body->body != source->body->body, "Token body was not deeply copied!");
    assert(copy->body->equals(copy->body, source->body) && copy->t_type == source->t_type, "Copy lost token content or type!");
    assert(copy->flags.ptr == 2 && copy->flags.ro && copy->flags.glob && copy->flags.ext && copy->flags.vla, "Copy lost token flags!");
    assert(copy->finfo.line == 7 && copy->finfo.column == 11 && copy->finfo.file == filename, "Copy lost source position!");
    assert(copy->body->rcat(copy->body, "_copy"), "Copied body mutation failed!");
    assert(source->body->requals(source->body, "name"), "Copy mutation affected source body!");
    copy->flags.ptr = 3;
    copy->finfo.line = 100;
    assert(source->flags.ptr == 2 && source->finfo.line == 7, "Copy mutation affected source metadata!");
    assert(TKN_unload_token(source), "Source cleanup failed!");
    assert(copy->body->requals(copy->body, "name_copy"), "Copy did not survive source cleanup!");
    assert(TKN_unload_token(copy), "Copy cleanup failed!");
    assert(filename->requals(filename, "copy.cpl"), "Copy cleanup freed borrowed filename!");
    destroy_string(filename);

    source = TKN_create_token(EOF_TOKEN, NULL, NULL);
    assert(source, "Bodyless source allocation failed!");
    copy = TKN_copy_token(source);
    assert(copy && copy != source && copy->t_type == EOF_TOKEN && !copy->body, "Bodyless token copy failed!");
    TKN_unload_token(copy);
    assert(source->t_type == EOF_TOKEN && !source->body, "Copy cleanup affected bodyless source!");
    TKN_unload_token(source);
    assert(!TKN_copy_token(NULL), "NULL token copy unexpectedly succeeded!");
    return 0;
}

static int _test_hash() {
    token_t* source = TKN_create_token(UNKNOWN_STRING_TOKEN, "name", NULL);
    assert(source, "Hash source creation failed!");
    token_t* copy = TKN_copy_token(source);
    assert(copy, "Hash copy creation failed!");
    unsigned long hash = TKN_hash_token(source, 0);
    assert(hash == TKN_hash_token(copy, 0), "Identical tokens have different hashes!");
    copy->finfo.line = 999;
    copy->finfo.column = 888;
    assert(hash == TKN_hash_token(copy, 0), "Source position affected token hash!");
    assert(copy->finfo.line == 999 && copy->finfo.column == 888, "Hash changed source position!");
    assert(copy->body->rcat(copy->body, "_other"), "Hash body mutation failed!");
    assert(hash != TKN_hash_token(copy, 0), "Changed body did not affect token hash!");
    assert(TKN_hash_token(source, 1) == TKN_hash_token(copy, 1), "Body affected hash with no_body enabled!");
    copy->t_type = STRING_VALUE_TOKEN;
    assert(TKN_hash_token(source, 1) != TKN_hash_token(copy, 1), "Token type did not affect hash!");
    copy->t_type = source->t_type;
    copy->flags.ptr = 1;
    assert(TKN_hash_token(source, 1) != TKN_hash_token(copy, 1), "Token flags did not affect hash!");
    TKN_unload_token(copy);
    TKN_unload_token(source);
    return 0;
}

int main() {
    mm_init();
    assert(!_test_copy(), "Token copy test failed!");
    assert(!_test_hash(), "Token hash test failed!");
    assert(mm_get_allocated() == 0, "Token copy tests leaked memory!");
    return 0;
}
