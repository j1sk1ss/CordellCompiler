#include <std/str.h>
#include "../../misc/testing.h"

int main() {
    mm_init();
    assert(str_strlen("hello") == 5, "Raw strlen failed!");

    string_t* s = create_string("hello");
    string_t* t = create_string("hello");
    string_t* u = create_string("world");
    assert(s && t && u, "String creation failed!");
    assert(s->len(s) == 5, "String length failed!");
    assert(s->equals(s, t), "String equality failed!");
    assert(!s->equals(s, u), "Different strings are equal!");
    assert(s->requals(s, "hello"), "Raw string equality failed!");
    assert(!s->requals(s, "hell"), "Raw different string is equal!");
    assert(s->index_of(s, 'e') == 1, "Index of existing char failed!");
    assert(s->index_of(s, 'x') == -1, "Index of missing char failed!");

    string_t* found = s->fchar(s, 'l');
    assert(found && found->requals(found, "llo"), "Find char failed!");
    destroy_string(found);
    assert(!s->fchar(s, 'x'), "Find missing char succeeded!");

    string_t* copy = s->copy(s);
    assert(copy && copy->equals(copy, s), "String copy failed!");
    assert(s->rcat(s, " world"), "Raw concat failed!");
    assert(s->requals(s, "hello world"), "Raw concat content failed!");
    assert(copy->cat(copy, u), "String concat failed!");
    assert(copy->requals(copy, "helloworld"), "String concat content failed!");

    assert(s->replace(s, "world", "C") == 1, "Replace count failed!");
    assert(s->requals(s, "hello C"), "Replace content failed!");
    assert(s->replace(s, "lo C", "p") == 1, "Partial match replace failed!");
    assert(s->requals(s, "help"), "Partial match replace content failed!");
    assert(s->replace(s, "lp", "LP") == 1, "Trailing replace failed!");
    assert(s->requals(s, "heLP"), "Trailing replace content failed!");
    assert(s->replace(s, "", "x") == 0, "Empty source replace succeeded!");
    assert(s->replace(s, "missing", "x") == 0, "Missing replace succeeded!");
    assert(s->hmove(s, 2), "Head move failed!");
    assert(s->requals(s, "LP"), "Moved head content failed!");
    assert(s->rhead(s), "Head reset failed!");
    assert(s->requals(s, "heLP"), "Reset head content failed!");

    string_t* c0 = create_string_from_int(0);
    string_t* c5 = create_string_from_int(205);
    assert(c0 && c0->requals(c0, "0"), "String from zero failed!");
    assert(c5 && c5->requals(c5, "205"), "String from int failed!");

    string_t* part = create_string_from_part("abcdef", 2, 3);
    assert(part && part->len(part) == 3, "Partial string length failed!");
    assert(part->requals(part, "cde"), "Partial string content failed!");

    destroy_string(part);
    destroy_string(c5);
    destroy_string(c0);
    destroy_string(copy);
    destroy_string(s);
    destroy_string(t);
    destroy_string(u);
    assert(!destroy_string(NULL), "Destroy NULL succeeded!");

    string_t *file_1 = create_string("FILE"), *file_2 = create_string("__FILE__");
    assert(file_1->hash != file_2->hash, "FILE == __FILE__ by hash compare");
    destroy_string(file_1);
    destroy_string(file_2); 
    return 0;
}
