#include <varmem.h>

varmem_ctx_t* VRM_create_ctx() {
    varmem_ctx_t* ctx = (varmem_ctx_t*)mm_malloc(sizeof(varmem_ctx_t));
    if (!ctx) return NULL;
    str_memset(ctx, 0, sizeof(varmem_ctx_t));
    return ctx;
}

int VRM_destroy_ctx(varmem_ctx_t* ctx) {
    if (!ctx) return 0;
    mm_free(ctx);
    return 1;
}

int VRM_get_info(const char* variable, const char* scope, variable_info_t* info, varmem_ctx_t* ctx) {
    variable_info_t* h = ctx->h;
    while (h) {
        if (((!scope && !h->scope[0]) || !str_strcmp(scope, h->scope)) && !str_strcmp(variable, h->name)) {
            if (info) str_memcpy(info, h, sizeof(variable_info_t));
            return 1;
        }

        h = h->next;
    }
    
    return 0;
}

static variable_info_t* _create_variable_info(const char* name, int size, const char* scope, varmem_ctx_t* ctx) {
    variable_info_t* var = (variable_info_t*)mm_malloc(sizeof(variable_info_t));
    if (!var) return NULL;
    str_memset(var, 0, sizeof(variable_info_t));

    if (scope) str_strncpy(var->scope, scope, TOKEN_MAX_SIZE);
    if (name)  str_strncpy(var->name, name, TOKEN_MAX_SIZE);

    ctx->offset = ALIGN(ctx->offset + size);
    var->offset = ctx->offset;
    var->size = size;
    var->next = NULL;
    return var;
}

int VRM_add_info(const char* name, int size, const char* scope, varmem_ctx_t* ctx) {
    variable_info_t* nnd = _create_variable_info(name, size, scope, ctx);
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

int VRM_unload(varmem_ctx_t* ctx) {
    variable_info_t* h = ctx->h;
    while (h) {
        variable_info_t* n = h->next;
        mm_free(h);
        h = n;
    }

    return 1;
}
