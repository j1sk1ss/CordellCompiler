#include <arrmem.h>

arrmem_ctx_t* ARM_create_ctx() {
    arrmem_ctx_t* ctx = (arrmem_ctx_t*)mm_malloc(sizeof(arrmem_ctx_t));
    if (!ctx) return NULL;
    str_memset(ctx, 0, sizeof(arrmem_ctx_t));
    return ctx;
}

int ARM_destroy_ctx(arrmem_ctx_t* ctx) {
    if (!ctx) return 0;
    mm_free(ctx);
    return 1;
}

int ARM_get_info(const char* name, short scope, array_info_t* info, arrmem_ctx_t* ctx) {
    array_info_t* h = ctx->h;
    while (h) {
        if (!str_strcmp(h->name, (char*)name) && ((scope < 0) || scope == h->scope)) {
            if (info) str_memcpy(info, h, sizeof(array_info_t));
            return 1;
        }

        h = h->next;
    }

    return 0;
}

static array_info_t* _create_info_array_entry(const char* name, short scope, int el_size, int size) {
    array_info_t* entry = (array_info_t*)mm_malloc(sizeof(array_info_t));
    if (!entry) return NULL;
    str_memset(entry, 0, sizeof(array_info_t));

    entry->scope = scope;
    if (name) str_strncpy(entry->name, name, TOKEN_MAX_SIZE);
    
    entry->el_size = el_size;
    entry->size = size;
    entry->next = NULL;
    return entry;
}

int ARM_add_info(const char* name, short scope, int el_size, int size, arrmem_ctx_t* ctx) {
    array_info_t* nnd = _create_info_array_entry(name, scope, el_size, size);
    if (!nnd) return 0;

    if (!ctx->h) {
        ctx->h = nnd;
        return 1;
    }

    array_info_t* h = ctx->h;
    while (h->next) {
        h = h->next;
    }

    h->next = nnd;
    return 1;
}

int ARM_unload(arrmem_ctx_t* ctx) {
    array_info_t* h = ctx->h;
    while (h) {
        array_info_t* n = h->next;
        mm_free(h);
        h = n;
    }

    return 1;
}
