#ifndef VARMEM_H_
#define VARMEM_H_

#include <std/regs.h>
#include <std/vars.h>
#include <prep/token.h>

typedef struct variable_info {
    long                  v_id;
    short                 s_id;
    char                  heap; /* Point to heap, can't be reused   */
    char                  ptr;  /* PTR type == maximum size in arch */
    char                  ro;
    char                  glob;
    char                  name[TOKEN_MAX_SIZE];
    token_type_t          type;
    struct variable_info* next;
    long                  offset;
} variable_info_t;

typedef struct {
    long             curr_id;
    variable_info_t* h;
} vartab_ctx_t;

int VRTB_update_offset(long id, long offset, vartab_ctx_t* ctx);
int VRTB_get_info_id(long id, variable_info_t* info, vartab_ctx_t* ctx);
int VRTB_get_info(const char* vname, short scope, variable_info_t* info, vartab_ctx_t* ctx);
int VRTB_add_info(const char* name, token_type_t type, short scope, token_flags_t* flags, vartab_ctx_t* ctx);
int VRTB_unload(vartab_ctx_t* ctx);

#endif