#include <std/set.h>
#include "../../misc/testing.h"

int main() {
    mm_init();
    set_t a, b, c;
    void* value = NULL;

    set_init(&a, SET_CMP);
    set_init(&b, SET_CMP);
    assert(set_is_init(&a), "Set isn't initialized!");
    assert(set_size(&a) == 0, "New set isn't empty!");
    assert(!set_has(&a, (void*)1), "Missing set value exists!");

    set_add(&a, (void*)1);
    set_add(&a, (void*)2);
    set_add(&a, (void*)3);
    set_add(&b, (void*)1);
    set_add(&b, (void*)2);
    assert(set_subset(&a, &b), "Subset check failed!");
    assert(!set_subset(&b, &a), "Reverse subset succeeded!");
    set_add(&b, (void*)99);
    assert(!set_subset(&a, &b), "Subset with missing value succeeded!");

    set_iter_t it;
    int count = 0;
    assert(set_iter_init(&a, &it), "Set iterator init failed!");
    while (set_iter_next(&it, &value)) count++;
    assert(count == set_size(&a), "Set iterator count failed!");

    int_tuple_t* t = inttuple_create(4, 5);
    int_tuple_t* same = inttuple_create(4, 5);
    int_tuple_t* other = inttuple_create(5, 4);
    set_init(&c, SET_NO_CMP);
    set_add(&c, t);
    assert(set_has_inttuple(&c, same), "Int tuple wasn't found!");
    assert(!set_has_inttuple(&c, other), "Different int tuple was found!");
    set_free(&c);
    inttuple_free(t);
    inttuple_free(same);
    inttuple_free(other);

    set_t copy;
    assert(set_copy(&copy, &a), "Set copy failed!");
    assert(set_equal(&copy, &a), "Copied set isn't equal!");
    set_add(&copy, (void*)42);
    assert(!set_equal(&copy, &a), "Different sets are equal!");
    set_free(&copy);

    set_t owned;
    set_init(&owned, SET_NO_CMP);
    int* p = (int*)mm_malloc(sizeof(int));
    assert(p, "Allocation failed!");
    assert(set_add(&owned, p), "Owned set add failed!");
    assert(set_free_force(&owned), "Set force free failed!");

    set_free(&a);
    set_free(&b);
    return 0;
}
