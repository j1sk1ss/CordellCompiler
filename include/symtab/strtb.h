#ifndef STRTB_H_
#define STRTB_H_

#include <std/str.h>
#include <std/map.h>

typedef enum {
    STR_RAW_ASM,
    STR_ARRAY_VALUE,
    STR_INDEPENDENT,
} str_type_t;

typedef struct {
    long       id;
    char       value[128];
    str_type_t t;
} str_info_t;

typedef struct {
    long  curr_id;
    map_t strtb;
} strtb_ctx_t;

int STTB_add_info(const char* value, str_type_t t, strtb_ctx_t* ctx);
int STTB_get_info_id(long id, str_info_t* info, strtb_ctx_t* ctx);
int STTB_get_info(const char* value, str_info_t* info, strtb_ctx_t* ctx);
int STTB_unload(strtb_ctx_t* ctx);

#endif