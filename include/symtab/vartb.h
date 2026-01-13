#ifndef VARMEM_H_
#define VARMEM_H_

#include <std/str.h>
#include <std/map.h>
#include <prep/token_types.h>
#include <symtab/symtab_id.h>

typedef struct {
    symbol_id_t            v_id;     /* Variable ID                      */
    symbol_id_t            p_id;     /* Parent variable ID (def: -1)     */
    short                  s_id;     /* Scope ID                         */
    string_t*              name;    
    token_type_t           type;     /* Variable type                    */

    struct {
        char               heap : 1; /* Point to heap, can't be reused   */
        char               ptr  : 1; /* PTR type == maximum size in arch */
        char               ro   : 1; /* Declaration RO flag              */
        char               glob : 1; /* Declaration global flag          */
    } vfs; /* VariableFlags          */

    struct {
        long               offset;
        long               size;
        char               reg;
        char               allocated : 1;
    } vmi; /* VariableMemoryInfo     */

    struct {
        long               definition;
        char               defined : 1;
    } vdi; /* VariableDefinitionInfo */
} variable_info_t;

typedef struct {
    symbol_id_t curr_id;
    map_t       vartb;
} vartab_ctx_t;

int VRTB_update_memory(symbol_id_t id, long offset, long size, char reg, vartab_ctx_t* ctx);
int VRTB_update_definition(symbol_id_t id, long definition, vartab_ctx_t* ctx);
int VRTB_get_info_id(symbol_id_t id, variable_info_t* info, vartab_ctx_t* ctx);
int VRTB_get_info(string_t* vname, short scope, variable_info_t* info, vartab_ctx_t* ctx);
symbol_id_t VRTB_add_copy(variable_info_t* src, vartab_ctx_t* ctx);
symbol_id_t VRTB_add_info(string_t* name, token_type_t type, short scope, token_flags_t* flags, vartab_ctx_t* ctx);
int VRTB_unload(vartab_ctx_t* ctx);

#endif