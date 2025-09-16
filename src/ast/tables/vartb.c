#include <vartb.h>

vartab_ctx_t* VRT_create_ctx() {
    vartab_ctx_t* ctx = (vartab_ctx_t*)mm_malloc(sizeof(vartab_ctx_t));
    if (!ctx) return NULL;
    str_memset(ctx, 0, sizeof(vartab_ctx_t));
    return ctx;
}

int VRT_destroy_ctx(vartab_ctx_t* ctx) {
    if (!ctx) return 0;
    mm_free(ctx);
    return 1;
}

int VRT_get_info(const char* varname, short scope, variable_info_t* info, vartab_ctx_t* ctx) {
    variable_info_t* h = ctx->h;
    while (h) {
        if (((scope < 0) || scope == h->scope) && !str_strcmp(varname, h->name)) {
            if (info) str_memcpy(info, h, sizeof(variable_info_t));
            return 1;
        }

        h = h->next;
    }
    
    return 0;
}

int VRT_update_value(const char* varname, short scope, const char* value, vartab_ctx_t* ctx) {
    variable_info_t* h = ctx->h;
    while (h) {
        if (((scope < 0) || scope == h->scope) && !str_strcmp(varname, h->name)) {
            str_strcpy(h->value, value);
            return 1;
        }

        h = h->next;
    }
    
    return 0;
}

static variable_info_t* _create_variable_info(
    const char* name, int size, char ro, char glob, short scope, int heap, vartab_ctx_t* ctx
) {
    variable_info_t* var = (variable_info_t*)mm_malloc(sizeof(variable_info_t));
    if (!var) return NULL;
    str_memset(var, 0, sizeof(variable_info_t));

    var->scope = scope;
    if (name) {
        str_strncpy(var->name, name, TOKEN_MAX_SIZE);
    }

    if (!ro && !glob) {
        ctx->offset = ALIGN(ctx->offset + size);
        var->offset = ctx->offset;
    }

    var->heap = heap;
    var->size = size;
    var->next = NULL;
    return var;
}

int VRT_add_info(const char* name, int size, char ro, char glob, short scope, int heap, vartab_ctx_t* ctx) {
    variable_info_t* nnd = _create_variable_info(name, size, ro, glob, scope, heap, ctx);
    if (!nnd) return 0;
    if (!ctx->h) {
        ctx->h = nnd;
        return nnd->offset;
    }

    variable_info_t* h = ctx->h;
    while (h->next) {
        h = h->next;
    }

    h->next = nnd;
    return nnd->offset;
}

int VRT_unload(vartab_ctx_t* ctx) {
    variable_info_t* h = ctx->h;
    while (h) {
        variable_info_t* n = h->next;
        mm_free(h);
        h = n;
    }

    return 1;
}
