#ifndef ARRMEM_H_
#define ARRMEM_H_

#include <std/str.h>
#include <prep/token.h>

typedef struct array_info {
    long               v_id;
    short              s_id;
    token_type_t       el_type;
    char               name[TOKEN_MAX_SIZE];
    struct array_info* next;
} array_info_t;

typedef struct {
    long          curr_id;
    array_info_t* h;
} arrtab_ctx_t;

int ARTB_add_info(const char* name, short scope, token_type_t el_type, arrtab_ctx_t* ctx);
int ARTB_get_info(const char* name, short scope, array_info_t* info, arrtab_ctx_t* ctx);
int ARTB_unload(arrtab_ctx_t* ctx);

#endif