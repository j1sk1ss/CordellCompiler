#ifndef ARRMEM_H_
#define ARRMEM_H_

#include "str.h"
#include "token.h"

typedef struct array_info {
    int                size;
    int                el_size;
    char               func[TOKEN_MAX_SIZE];
    char               name[TOKEN_MAX_SIZE];
    struct array_info* next;
} array_info_t;

typedef struct {
    array_info_t* h;
} arrmem_ctx_t;

arrmem_ctx_t* ARM_create_ctx();
int ARM_destroy_ctx(arrmem_ctx_t* ctx);

int ARM_add_info(const char* name, const char* func, int el_size, int size, arrmem_ctx_t* ctx);
int ARM_get_info(const char* name, const char* func, array_info_t* info, arrmem_ctx_t* ctx);
int ARM_unload(arrmem_ctx_t* ctx);

#endif