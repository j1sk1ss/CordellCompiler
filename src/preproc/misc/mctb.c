#include <preproc/mctb.h>

static define_t* _create_def(char* n, char* def) {
    define_t* d = (define_t*)mm_malloc(sizeof(define_t));
    if (!d) return NULL;
    d->name  = create_string(n);
    d->value = create_string(def);
    d->args  = NULL;
    if (!d->name || !d->value) {
        destroy_string(d->name);
        destroy_string(d->value);
        mm_free(d);
        return NULL;
    }

    return d;
}

static int _free_positions(list_t* positions) {
    list_free(positions);
    mm_free(positions);
    return 1;
}

int MCTB_free_macro_args(macro_args_t* args) {
    if (!args) return 1;
    list_free_force_op(&args->args, (int (*)(void*))destroy_string);
    if (args->positions.entries) {
        map_free_force_op(&args->positions, (int (*)(void*))_free_positions);
    }

    mm_free(args);
    return 1;
}

static int _unload_def(define_t* d) {
    if (!d) return 0;
    destroy_string(d->name);
    destroy_string(d->value);
    MCTB_free_macro_args(d->args);
    mm_free(d);
    return 1;
}

int MCTB_put_define(char* name, char* value, deftb_t* ctx) {
    return MCTB_put_macro(name, value, NULL, ctx);
}

int MCTB_put_macro(char* name, char* value, macro_args_t* args, deftb_t* ctx) {
    if (MCTB_get_define(name, NULL, ctx)) {
        MCTB_free_macro_args(args);
        return 1;
    }

    define_t* d = _create_def(name, value);
    if (!d) return 0;
    if (!map_put(&ctx->t, (long)d->name->hash, d)) {
        _unload_def(d);
        return 0;
    }

    d->args = args;
    return 1;
}

int MCTB_remove_define(char* name, deftb_t* ctx) {
    string_t* sname = create_string(name);
    if (!sname) return 0;

    define_t* d;
    if (
        map_get(&ctx->t, (long)sname->hash, (void**)&d) && 
        map_remove(&ctx->t, (long)sname->hash)
    ) {
        _unload_def(d);
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
