#ifndef VARMEM_H_
#define VARMEM_H_

#include "regs.h"
#include "vars.h"
#include "token.h"

typedef struct variable_info {
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
} varmem_ctx_t;

/*
Create variable table context.
*/
varmem_ctx_t* VRM_create_ctx();

/*
Destroy variable table.
*/
int VRM_destroy_ctx(varmem_ctx_t* ctx);

/*
Get variable info from context by varname and function scope.
*/
int VRM_get_info(const char* vname, short scope, variable_info_t* info, varmem_ctx_t* ctx);

/*
Update value of variable by name.
*/
int VRM_update_value(const char* varname, short scope, const char* value, varmem_ctx_t* ctx);

/*
Add variable to context. 
Note: Will use ALIGN to variable size for offset calculation.
*/
int VRM_add_info(const char* vname, int size, short scope, varmem_ctx_t* ctx);

/*
Unload context.
*/
int VRM_unload(varmem_ctx_t* ctx);

#endif