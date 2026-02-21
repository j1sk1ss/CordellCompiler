#ifndef STRTB_H_
#define STRTB_H_

#include <std/mm.h>
#include <std/str.h>
#include <std/map.h>
#include <symtab/symtab_id.h>

typedef enum {
    STR_RAW_ASM,     // ASM line from the inline assembly 
    STR_ARRAY_VALUE, // non RO-string
    STR_INDEPENDENT, // RO-string
    STR_COMMENT,     // Unchanged string for a debug
} str_type_t;

typedef struct {
    symbol_id_t id;
    string_t*   value;
    str_type_t  t;
} str_info_t;

typedef struct {
    symbol_id_t curr_id;
    map_t       strtb;
} strtb_ctx_t;

int STTB_update_info(symbol_id_t id, string_t* value, str_type_t t, strtb_ctx_t* ctx);
symbol_id_t STTB_add_info(string_t* value, str_type_t t, strtb_ctx_t* ctx);
int STTB_get_info_id(symbol_id_t id, str_info_t* info, strtb_ctx_t* ctx);
int STTB_get_info(string_t* value, str_info_t* info, strtb_ctx_t* ctx);
int STTB_unload(strtb_ctx_t* ctx);

#endif