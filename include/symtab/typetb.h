#ifndef TYPETB_H_
#define TYPETB_H_

#include <std/mm.h>
#include <std/map.h>
#include <std/str.h>
#include <std/list.h>
#include <prep/token_types.h>
#include <symtab/symtab_id.h>

typedef enum {
    TYPE_GENERICS,  /* <T>       */
    TYPE_CUSTOM,    /* container */
    TYPE_PRIMITIVE, /* i32, i0.. */
} type_type_t;

typedef struct {
    symbol_id_t vid; /* Registered variable */
} type_entry_info_t;

typedef struct {
    list_t          entries; /* Primitive entries   */
    string_t*       name;    /* Type name           */
    symbol_id_t     s_id;
    symbol_id_t     id;
    type_type_t     t;       /* type's type         */
    token_type_t    tt;      /* primitive type      */
    struct {
        list_t      c;
        symbol_id_t p;
        string_t*   name;    /* Linked name         */
    } link;
} type_info_t;

typedef struct {
    symbol_id_t curr_id;
    map_t       typetb;
} typetab_ctx_t;

symbol_id_t TPTB_add_info(string_t* name, symbol_id_t s_id, type_type_t t, typetab_ctx_t* ctx);
symbol_id_t TPTB_add_info_from_token_type(symbol_id_t s_id, token_type_t t, typetab_ctx_t* ctx);
int TPTB_add_as_child(symbol_id_t p_id, symbol_id_t c_id, string_t* name, typetab_ctx_t* ctx);
int TPTB_info_add_entry(symbol_id_t id, symbol_id_t vid, typetab_ctx_t* ctx);
int TPTB_get_info_id(symbol_id_t id, type_info_t* info, typetab_ctx_t* ctx);
type_type_t TPTB_get_type_type_id(symbol_id_t id, typetab_ctx_t* ctx);
int TPTB_get_info(string_t* name, symbol_id_t s_id, type_info_t* info, typetab_ctx_t* ctx);
int TPTB_unload(typetab_ctx_t* ctx);

#endif