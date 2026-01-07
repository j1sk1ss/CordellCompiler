#include <preproc/mctb.h>

static define_t* _create_def(char* n, char* def) {
    define_t* d = (define_t*)mm_malloc(sizeof(define_t));
    if (!d) return NULL;
    d->name  = create_string(n);
    d->value = create_string(def);
    return d;
}

static int _unload_def(define_t* d) {
    if (!d) return 0;
    destroy_string(d->name);
    destroy_string(d->value);
    mm_free(d);
    return 1;
}

int MCTB_put_define(char* name, char* value, deftb_t* ctx) {
    if (MCTB_get_define(name, NULL, ctx)) return 1;
    define_t* d = _create_def(name, value);
    if (!d) return 0;

    if (!map_put(&ctx->t, (long)d->name->hash, d)) {
        _unload_def(d);
        return 0;
    }

    return 1;
}

int MCTB_remove_define(char* name, deftb_t* ctx) {
    string_t* sname = create_string(name);
    if (!sname) return 0;

    if (map_remove(&ctx->t, (long)sname->hash)) {
        destroy_string(sname);
        return 1;
    }

    destroy_string(sname);
    return 0;
}

int MCTB_get_define(char* name, define_t* out, deftb_t* ctx) {
    string_t* sname = create_string(name);
    if (!sname) return 0;

    define_t* d;
    if (map_get(&ctx->t, (long)sname->hash, (void**)&d)) {
        destroy_string(sname);
        if (out) str_memcpy(out, d, sizeof(define_t));
        return 1;
    }

    destroy_string(sname);
    return 0;
}

int MCTB_init(deftb_t* ctx) {
    return map_init(&ctx->t, MAP_NO_CMP);
}

int MCTB_unload(deftb_t* ctx) {
    return map_free_force_op(&ctx->t, (int (*)(void*))_unload_def);
}
