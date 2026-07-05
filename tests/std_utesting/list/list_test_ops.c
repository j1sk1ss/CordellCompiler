#include <std/list.h>
#include "../../misc/testing.h"

static int freed = 0;

static int _free_count(void* p) {
    freed++;
    return mm_free(p);
}

int main() {
    mm_init();
    list_t l;
    list_t copy;
    list_iter_t it;
    void* data = NULL;

    list_init(&l);
    assert(list_push_back(&l, (void*)9) == 0, "Push back into empty list failed!");
    assert(list_pop_front(&l) == (void*)9, "Single element pop failed!");
    assert(list_insert(&l, (void*)8, (void*)9) == 0, "Insert into empty list failed!");
    assert(list_size(&l) == 1 && list_get_head(&l) == (void*)8, "Empty insert result failed!");
    list_free(&l);

    list_init(&l);
    assert(list_size(&l) == 0, "New list isn't empty!");
    assert(list_get_head(&l) == NULL, "Empty head isn't NULL!");
    assert(list_get_tail(&l) == NULL, "Empty tail isn't NULL!");
    assert(list_pop_front(&l) == NULL, "Pop from empty list succeeded!");
    assert(!list_remove(&l, (void*)1), "Remove from empty list succeeded!");
    assert(!list_replace(&l, (void*)1, (void*)2), "Replace in empty list succeeded!");

    assert(list_push_front(&l, (void*)2) == 0, "Push front failed!");
    assert(list_push_back(&l, (void*)3) == 0, "Push back failed!");
    assert(list_insert(&l, (void*)1, (void*)2) == 0, "Insert before head failed!");
    assert(list_insert(&l, (void*)4, (void*)99) == 0, "Insert missing target append failed!");
    assert(list_size(&l) == 4, "List size after inserts failed!");
    assert(list_get_head(&l) == (void*)1, "List head failed!");
    assert(list_get_tail(&l) == (void*)4, "List tail failed!");
    assert(list_replace(&l, (void*)3, (void*)33), "List replace failed!");
    assert(!list_replace(&l, (void*)99, (void*)5), "Missing replace succeeded!");
    assert(list_remove(&l, (void*)4), "Tail remove failed!");
    assert(list_push_back(&l, (void*)4) == 0, "Tail restore failed!");

    assert(list_iter_hinit(&l, &it), "Head iterator init failed!");
    assert(list_iter_current(&it) == (void*)1, "Iterator current failed!");
    assert(list_iter_next_top(&it) == (void*)2, "Iterator next top failed!");
    assert(list_iter_set(&it, (void*)11), "Iterator set failed!");
    assert(list_iter_next(&it, &data) && data == (void*)11, "Iterator next first failed!");
    assert(list_iter_next(&it, NULL), "Iterator next with NULL output failed!");
    assert(list_iter_next(&it, &data) && data == (void*)33, "Iterator next third failed!");
    assert(list_iter_next(&it, &data) && data == (void*)4, "Iterator next fourth failed!");
    assert(!list_iter_next(&it, &data), "Iterator passed end!");
    assert(list_iter_current(&it) == NULL, "End iterator current isn't NULL!");
    assert(!list_iter_set(&it, (void*)1), "End iterator set succeeded!");
    assert(list_iter_next_top(&it) == NULL, "End iterator next top isn't NULL!");

    assert(list_iter_tinit(&l, &it), "Tail iterator init failed!");
    assert(list_iter_prev_top(&it) == (void*)33, "Iterator prev top failed!");
    assert(list_iter_prev(&it) == (void*)4, "Iterator prev first failed!");
    assert(list_iter_prev(&it) == (void*)33, "Iterator prev second failed!");
    assert(list_iter_prev(&it) == (void*)2, "Iterator prev third failed!");
    assert(list_iter_prev(&it) == (void*)11, "Iterator prev fourth failed!");
    assert(list_iter_prev(&it) == NULL, "Iterator prev passed end!");
    assert(list_iter_prev_top(&it) == NULL, "End iterator prev top isn't NULL!");

    void** flat = list_flatten(&l);
    assert(flat, "List flatten failed!");
    assert(flat[0] == (void*)11 && flat[3] == (void*)4, "Flatten content failed!");
    mm_free(flat);

    list_init(&copy);
    assert(list_copy(&l, &copy), "List copy failed!");
    assert(list_size(&copy) == list_size(&l), "List copy size failed!");
    assert(list_pop_front(&copy) == (void*)11, "Pop copied list failed!");
    list_free(&copy);
    list_free(&l);

    assert(list_insert(NULL, (void*)1, (void*)2) == -1, "NULL insert accepted!");

    list_init(&l);
    for (int i = 0; i < 3; i++) {
        int* p = (int*)mm_malloc(sizeof(int));
        assert(p, "Allocation failed!");
        *p = i;
        assert(list_add(&l, p), "Pointer list add failed!");
    }
    assert(list_free_force_op(&l, _free_count), "List force free op failed!");
    assert(freed == 3, "List force free op didn't visit all elements!");

    list_init(&l);
    int* p = (int*)mm_malloc(sizeof(int));
    assert(p, "Allocation failed!");
    assert(list_add(&l, p), "Pointer list add failed!");
    assert(list_free_force(&l), "List default force free failed!");
    return 0;
}
