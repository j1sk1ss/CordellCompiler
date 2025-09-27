#ifndef STRTB_H_
#define STRTB_H_

#include <std/str.h>
#include <std/list.h>
#include <prep/token.h>

typedef struct {
    int  id;
    char value[TOKEN_MAX_SIZE];
} str_info_t;

typedef struct {
    long   curr_id;
    list_t lst;
} strtb_ctx_t;

int STTB_add_info(const char* name, strtb_ctx_t* ctx);
int STTB_get_info_id(long id, str_info_t* info, strtb_ctx_t* ctx);
int STTB_get_info(const char* name, str_info_t* info, strtb_ctx_t* ctx);
int STTB_unload(strtb_ctx_t* ctx);

#endif