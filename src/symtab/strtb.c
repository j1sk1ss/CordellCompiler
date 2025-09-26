#include <symtab/strtb.h>

int STTB_add_info(const char* name, strtb_ctx_t* ctx) {
    print_debug("STTB_add_info(name=%s)", name);
    str_info_t* node = (str_info_t*)mm_malloc(sizeof(str_info_t));
    if (!node) return 0;
    node->id = ctx->curr_id++;
    if (name) str_strncpy(node->value, name, TOKEN_MAX_SIZE);
    node->next = ctx->h;
    ctx->h = node;
    return node->id;
}

int STTB_get_info_id(long id, str_info_t* info, strtb_ctx_t* ctx) {
    if (!ctx || !info) return 0;
    str_info_t* cur = ctx->h;
    while (cur) {
        if (cur->id == id) {
            str_memcpy(info, cur, sizeof(str_info_t));
            info->next = NULL;
            return 1;
        }

        cur = cur->next;
    }

    return 0;
}

int STTB_get_info(const char* name, str_info_t* info, strtb_ctx_t* ctx) {
    if (!ctx || !name || !info) return 0;
    str_info_t* cur = ctx->h;
    while (cur) {
        if (!str_strncmp(cur->value, name, TOKEN_MAX_SIZE)) {
            str_memcpy(info, cur, sizeof(str_info_t));
            info->next = NULL;
            return 1;
        }

        cur = cur->next;
    }

    return 0;
}

int STTB_unload(strtb_ctx_t* ctx) {
    if (!ctx) return 0;
    str_info_t* cur = ctx->h;
    while (cur) {
        str_info_t* next = cur->next;
        mm_free(cur);
        cur = next;
    }

    ctx->h = NULL;
    ctx->curr_id = 0;
    return 0;
}
