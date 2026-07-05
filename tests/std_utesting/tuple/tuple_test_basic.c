#include <std/tuple.h>
#include "../../misc/testing.h"

int main() {
    mm_init();
    int_tuple_t* t = inttuple_create(12, -34);
    assert(t, "Tuple wasn't created!");
    assert(t->x == 12 && t->y == -34, "Tuple fields are incorrect!");
    assert(inttuple_free(t), "Tuple wasn't freed!");
    return 0;
}
