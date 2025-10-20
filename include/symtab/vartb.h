#ifndef VARMEM_H_
#define VARMEM_H_

#include <std/map.h>
#include <prep/token_types.h>

typedef struct {
    long offset;
    long size;
    char reg;
    char allocated;
} variable_memory_info_t;

typedef struct {
    long                   v_id; /* Variable ID                      */
    long                   p_id; /* Parent variable ID (def: -1)     */
    short                  s_id; /* Scope ID                         */
    char                   heap; /* Point to heap, can't be reused   */
    char                   ptr;  /* PTR type == maximum size in arch */
    char                   ro;
    char                   glob;
    char                   name[TOKEN_MAX_SIZE];
    token_type_t           type;
    variable_memory_info_t vmi;
} variable_info_t;

typedef struct {
    long  curr_id;
    map_t vartb;
} vartab_ctx_t;

int VRTB_update_memory(long id, long offset, long size, char reg, vartab_ctx_t* ctx);
int VRTB_get_info_id(long id, variable_info_t* info, vartab_ctx_t* ctx);
int VRTB_get_info(const char* vname, short scope, variable_info_t* info, vartab_ctx_t* ctx);
int VRTB_add_copy(variable_info_t* src, vartab_ctx_t* ctx);
int VRTB_add_info(const char* name, token_type_t type, short scope, token_flags_t* flags, vartab_ctx_t* ctx);
int VRTB_unload(vartab_ctx_t* ctx);

#endif