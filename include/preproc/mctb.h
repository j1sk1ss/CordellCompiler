#ifndef MCTB_H_
#define MCTB_H_

#include <std/mm.h>
#include <std/map.h>
#include <std/str.h>
#include <std/list.h>

typedef struct {
    list_t        args;
    map_t         positions;
} macro_args_t;

typedef struct {
    string_t*     name;
    string_t*     value;
    macro_args_t* args;
} define_t;

typedef struct {
    map_t         t;
} deftb_t;

int MCTB_put_define(char* name, char* value, deftb_t* ctx);
int MCTB_put_macro(char* name, char* value, macro_args_t* args, deftb_t* ctx);
int MCTB_free_macro_args(macro_args_t* args);
int MCTB_remove_define(char* name, deftb_t* ctx);
int MCTB_get_define(char* name, define_t* out, deftb_t* ctx);
int MCTB_init(deftb_t* ctx);
int MCTB_unload(deftb_t* ctx);

#endif