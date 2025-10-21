#ifndef ARRMEM_H_
#define ARRMEM_H_

#include <std/str.h>
#include <std/map.h>
#include <std/list.h>
#include <prep/token_types.h>

typedef struct {
    char         heap;
    long         v_id;
    long         size;
    list_t       elems;
    token_type_t el_type;
} array_info_t;

typedef struct {
    map_t arrtb;
} arrtab_ctx_t;

int ARTB_get_info(long id, array_info_t* info, arrtab_ctx_t* ctx);
list_t* ARTB_get_elems(long id, arrtab_ctx_t* ctx);
int ARTB_add_info(long id, long size, int heap, token_type_t el_type, arrtab_ctx_t* ctx);
int ARTB_unload(arrtab_ctx_t* ctx);

#endif