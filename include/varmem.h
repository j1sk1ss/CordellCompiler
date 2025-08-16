#ifndef VARMEM_H_
#define VARMEM_H_

#include "regs.h"
#include "vars.h"
#include "token.h"

typedef struct variable_info {
    int                   size;
    int                   offset;
    char                  name[TOKEN_MAX_SIZE];
    char                  func[TOKEN_MAX_SIZE];
    struct variable_info* next;
} variable_info_t;

typedef struct {
    unsigned int offset;
    variable_info_t* h;
} varmem_ctx_t;

varmem_ctx_t* VRM_create_ctx();
int VRM_destroy_ctx(varmem_ctx_t* ctx);

/*
Get variable info from context by varname and function scope.
*/
int VRM_get_info(const char* vname, const char* func, variable_info_t* info, varmem_ctx_t* ctx);

/*
Add variable to context. 
Note: Will use ALIGN to variable size for offset calculation.
*/
int VRM_add_info(const char* vname, int size, const char* func, varmem_ctx_t* ctx);

/*
Unload context.
*/
int VRM_unload(varmem_ctx_t* ctx);

#endif