#ifndef TYPETB_H_
#define TYPETB_H_

#include <std/mm.h>
#include <std/map.h>
#include <std/str.h>
#include <std/list.h>
#include <prep/token_types.h>
#include <symtab/symtab_id.h>

typedef enum {
    TYPE_GENERICS,  /* <T>             */
    TYPE_CUSTOM,    /* container       */
    TYPE_PRIMITIVE, /* i32, i0..       */
    TYPE_METHOD,    /* function + self */
    TYPE_ARRAY,     /* array           */
} type_type_t;

typedef struct {
    symbol_id_t      id;
    symbol_id_t      p;       /* parent of the copy  */

    string_t*        name;    /* Type name           */
    symbol_id_t      s_id;
    symbol_id_t      cs_id;   /* Child's scope Id    */
    type_type_t      t;       /* type's type         */
    token_type_t     tt;
    
    struct {
        long         size;    /* Type's size (hook)  */
        int          ptr;
        char         align;
        char         multiple;
    } memory;
    
    struct {
        list_t       c;
        symbol_id_t  p;
        string_t*    name;    /* Linked name         */
        symbol_id_t  v_id;
    } link;
} type_info_t;

typedef struct {
    symbol_id_t curr_id;
    map_t       typetb;
} typetab_ctx_t;

symbol_id_t TPTB_add_info(string_t* name, symbol_id_t s_id, type_type_t t, int align, int multiple, typetab_ctx_t* ctx);
symbol_id_t TPTB_add_copy(symbol_id_t id, symbol_id_t nv_id, int ptr, typetab_ctx_t* ctx);
symbol_id_t TPTB_add_info_from_token(symbol_id_t s_id, token_t* t, symbol_id_t v_id, typetab_ctx_t* ctx);
long TPTB_get_memory_size_id(symbol_id_t id, typetab_ctx_t* ctx);
int TPTB_set_memory_size_id(symbol_id_t id, long size, typetab_ctx_t* ctx);
int TPTB_link_child(symbol_id_t p_id, symbol_id_t c_id, typetab_ctx_t* ctx);
symbol_id_t TPTB_get_first_child(symbol_id_t p_id, typetab_ctx_t* ctx);
long TPTB_get_child_offset(symbol_id_t p_id, symbol_id_t tc_id, typetab_ctx_t* ctx);
int TPTB_add_as_child(symbol_id_t p_id, symbol_id_t c_id, string_t* name, long overrite_size, typetab_ctx_t* ctx);
int TPTB_get_info_id(symbol_id_t id, type_info_t* info, typetab_ctx_t* ctx);
symbol_id_t TPTB_resolve_child(symbol_id_t p_id, string_t* name, typetab_ctx_t* ctx);
type_type_t TPTB_get_type_type_id(symbol_id_t id, typetab_ctx_t* ctx);
token_type_t TPTB_get_token_type_id(symbol_id_t id, typetab_ctx_t* ctx);
int TPTB_get_info(string_t* name, symbol_id_t s_id, int ptr, type_info_t* info, typetab_ctx_t* ctx);
int TPTB_find_type_init_slot(
    symbol_id_t t_id, long target_slot, long base_offset, long* current_slot, long* slot_offset, long* slot_size, typetab_ctx_t* ctx
);
int TPTB_unload(typetab_ctx_t* ctx);

#endif
