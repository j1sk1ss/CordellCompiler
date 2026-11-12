#include <builder.h>

static int _unload_token_list(void* data) {
    list_t* tokens = (list_t*)data;
    if (!tokens) return 1;
    list_free_force_op(tokens, (int (*)(void *))TKN_unload_token);
    mm_free(tokens);
    return 1;
}

void BUILD_unload_token_lists(list_t* token_lists) {
    if (!token_lists) return;
    list_free_force_op(token_lists, _unload_token_list);
}
