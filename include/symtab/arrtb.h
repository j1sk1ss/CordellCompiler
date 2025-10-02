#ifndef ARRMEM_H_
#define ARRMEM_H_

#include <std/str.h>
#include <std/list.h>
#include <prep/token.h>

typedef struct {
    char         heap;
    long         v_id;
    token_type_t el_type;
} array_info_t;

typedef struct {
    list_t lst;
} arrtab_ctx_t;

int ARTB_get_info(long id, array_info_t* info, arrtab_ctx_t* ctx);
int ARTB_add_info(long id, int heap, token_type_t el_type, arrtab_ctx_t* ctx);
int ARTB_unload(arrtab_ctx_t* ctx);

#endif