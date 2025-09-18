#ifndef VARMEM_H_
#define VARMEM_H_

#include <std/regs.h>
#include <std/vars.h>
#include <prep/token.h>

typedef struct variable_info {
    char                  heap;
    char                  ro;
    char                  glob;
    char                  name[TOKEN_MAX_SIZE];
    char                  value[TOKEN_MAX_SIZE];
    short                 scope;
    int                   size;
    int                   offset;
    struct variable_info* next;
} variable_info_t;

typedef struct {
    unsigned int     offset;
    variable_info_t* h;
} vartab_ctx_t;

/*
Create variable table context.
*/
vartab_ctx_t* VRT_create_ctx();

/*
Destroy variable table.
*/
int VRT_destroy_ctx(vartab_ctx_t* ctx);

/*
Get variable info from context by varname and function scope.
*/
int VRT_get_info(const char* vname, short scope, variable_info_t* info, vartab_ctx_t* ctx);

/*
Update value of variable by name.
*/
int VRT_update_value(const char* varname, short scope, const char* value, vartab_ctx_t* ctx);

/*
Add variable to context. 
Note: Will use ALIGN to variable size for offset calculation.
*/
int VRT_add_info(const char* name, int size, short scope, token_flags_t* flags, vartab_ctx_t* ctx);

/*
Unload context.
*/
int VRT_unload(vartab_ctx_t* ctx);

#endif