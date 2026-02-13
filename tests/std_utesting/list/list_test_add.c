#include <std/list.h>
#include "../../misc/testing.h"

static void _fill_the_list(list_t* s) {
    long data[] = { 1, 2, 3, 432, 1253, 928838, 1, 1, 532, 54, 123, 653, 7 };
    for (int i = 0; i < (int)(sizeof(data) / sizeof(long)); i++) {
        list_add(s, (void*)data[i]);
    }
}

int main() {
    mm_init();
    list_t a;
    list_init(&a);

    _fill_the_list(&a);
    assert(list_size(&a) == 13, "List size isn't correct!");

    list_free(&a);
    return 0;
}
