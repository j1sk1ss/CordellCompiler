#ifndef VTTB_H_
#define VTTB_H_

#include <std/mm.h>
#include <std/map.h>
#include <std/list.h>
#include <std/str.h>
#include <symtab/symtab_id.h>

typedef struct {
    symbol_id_t id;
    symbol_id_t t_id;
    list_t      funcs; /* symbol_id_t */
} vtable_info_t;

typedef struct {
    symbol_id_t curr_id;
    map_t       vttb;
} vttab_ctx_t;

symbol_id_t VTTB_add_info(symbol_id_t t_id, vttab_ctx_t* ctx);
int         VTTB_add_func(symbol_id_t id, symbol_id_t f_id, vttab_ctx_t* ctx);
int         VTTB_get_info_id(symbol_id_t id, vtable_info_t* info, vttab_ctx_t* ctx);
int         VTTB_get_info_type(symbol_id_t t_id, vtable_info_t* info, vttab_ctx_t* ctx);
int         VTTB_unload(vttab_ctx_t* ctx);

#endif