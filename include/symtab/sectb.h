#ifndef SECTB_H_
#define SECTB_H_

#include <std/mm.h>
#include <std/map.h>
#include <std/set.h>
#include <std/str.h>
#include <std/list.h>
#include <symtab/symtab_id.h>

typedef enum {
    SECTION_ELEMENT_VARIABLE,
    SECTION_ELEMENT_FUNCTION,
    SECTION_ELEMENT_STRING
} section_elem_type_t;

typedef struct {
    string_t* name;
    set_t     vars; /* :symbol_id_t */
    set_t     func; /* :symbol_id_t */
    set_t     strs; /* :symbol_id_t */
} section_info_t;

typedef struct {
    map_t sectb;    /* :section_info_t */
} sectb_ctx_t;

int SCTB_move_to_section(string_t* section, symbol_id_t id, section_elem_type_t t, sectb_ctx_t* ctx);
int SCTB_add_to_section(string_t* section, symbol_id_t id, section_elem_type_t t, sectb_ctx_t* ctx);
int SCTB_remove_from_section(string_t* section, symbol_id_t id, section_elem_type_t t, sectb_ctx_t* ctx);
int SCTB_get_section(list_t* out, string_t* section, section_elem_type_t t, sectb_ctx_t* ctx);
int SCTB_get_sections(list_t* out, sectb_ctx_t* ctx);
int SCTB_unload(sectb_ctx_t* ctx);

#endif
