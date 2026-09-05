#ifndef VARMEM_H_
#define VARMEM_H_

#include <std/str.h>
#include <std/map.h>
#include <prep/token_types.h>
#include <symtab/symtab_id.h>

typedef struct {
    symbol_id_t         v_id;     /* Variable ID                      */
    symbol_id_t         p_id;     /* Parent variable ID (def: -1)     */
    symbol_id_t         s_id;     /* Scope ID                         */
    string_t*           name;    
    token_type_t        type;     /* Variable type                    */
    symbol_id_t         t_id;     /* Varaible's type ID               */
    basic_object_info_t vfs;

    struct {
        short           align;
        long            offset;
        long            size;
        char            reg;
        char            allocated : 1;
        char            used      : 1;
        char            vlatile   : 1;
    } vmi; /* VariableMemoryInfo      */

    struct {
        long            definition;
        char            defined;
    } vdi; /* VariableDefinitionInfo  */

    struct {
        char            not_null  : 1;
    } csa; /* CordellStaticAnalyzator */
} variable_info_t;

typedef struct {
    symbol_id_t curr_id;
    map_t       vartb;
} vartab_ctx_t;

#define UNDEFINED_VARIABLE   0
#define DEFINED_VARIABLE     1
#define OVERDEFINED_VARIABLE 2

int VRTB_set_used(symbol_id_t id, vartab_ctx_t* ctx);
int VRTB_set_volatile(symbol_id_t id, vartab_ctx_t* ctx);
int VRTB_set_not_null(symbol_id_t id, vartab_ctx_t* ctx);
int VRTB_update_memory(symbol_id_t id, long offset, long size, char reg, short align, vartab_ctx_t* ctx);
int VRTB_update_definition(symbol_id_t id, long definition, symbol_id_t overdefined, vartab_ctx_t* ctx, int rewrite);
int VRTB_update_type(symbol_id_t id, int t, symbol_id_t t_id, vartab_ctx_t* ctx);
int VRTB_get_info_id(symbol_id_t id, variable_info_t* info, vartab_ctx_t* ctx);
int VRTB_find_by_type_id(symbol_id_t t_id, variable_info_t* info, vartab_ctx_t* ctx);
int VRTB_find_by_type_id_name(symbol_id_t t_id, string_t* name, symbol_id_t s_id, variable_info_t* info, vartab_ctx_t* ctx);
int VRTB_get_info(string_t* vname, symbol_id_t scope, variable_info_t* info, vartab_ctx_t* ctx);
symbol_id_t VRTB_resolve_parent(symbol_id_t id, vartab_ctx_t* ctx);
symbol_id_t VRTB_add_copy(variable_info_t* src, vartab_ctx_t* ctx);
symbol_id_t VRTB_add_info(string_t* name, token_type_t type, symbol_id_t scope, basic_object_info_t flags, vartab_ctx_t* ctx);
int VRTB_unload(vartab_ctx_t* ctx);

#endif