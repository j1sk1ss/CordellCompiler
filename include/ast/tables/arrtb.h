#ifndef ARRMEM_H_
#define ARRMEM_H_

#include <std/str.h>
#include <prep/token.h>

typedef struct array_info {
    int                size;
    int                el_size;
    short              scope;
    char               name[TOKEN_MAX_SIZE];
    struct array_info* next;
} array_info_t;

typedef struct {
    array_info_t* h;
} arrtab_ctx_t;

arrtab_ctx_t* ART_create_ctx();
int ART_destroy_ctx(arrtab_ctx_t* ctx);
int ART_add_info(const char* name, short scope, int el_size, int size, arrtab_ctx_t* ctx);
int ART_get_info(const char* name, short scope, array_info_t* info, arrtab_ctx_t* ctx);
int ART_unload(arrtab_ctx_t* ctx);

#endif