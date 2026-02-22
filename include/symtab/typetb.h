#ifndef TYPETB_H_
#define TYPETB_H_

#include <std/mm.h>
#include <std/map.h>
#include <std/str.h>
#include <std/list.h>
#include <prep/token_types.h>
#include <symtab/symtab_id.h>

typedef struct {
    symbol_id_t vid; /* Registered variable */
} type_entry_info_t;

typedef struct {
    list_t      entries; /* Primitive entries   */
    string_t*   name;    /* Type name           */
    symbol_id_t id;
} type_info_t;

typedef struct {
    symbol_id_t curr_id;
    map_t       typetb;
} typetab_ctx_t;

symbol_id_t TPTB_add_info(string_t* name, typetab_ctx_t* ctx);
int TPTB_info_add_entry(symbol_id_t tid, symbol_id_t vid, typetab_ctx_t* ctx);
int TPTB_get_info_id(symbol_id_t tid, type_info_t* info, typetab_ctx_t* ctx);
int TPTB_get_info(string_t* name, type_info_t* info, typetab_ctx_t* ctx);
int TPTB_unload(typetab_ctx_t* ctx);

#endif
