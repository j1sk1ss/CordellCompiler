#ifndef MCTB_H_
#define MCTB_H_

#include <std/mm.h>
#include <std/map.h>
#include <std/str.h>

typedef struct {
    string_t* name;
    string_t* value;
} define_t;

typedef struct {
    map_t t;
} deftb_t;

int MCTB_init(deftb_t* ctx);
int MCTB_put_define(char* name, char* value, deftb_t* ctx);
int MCTB_remove_define(char* name, deftb_t* ctx);
int MCTB_get_define(char* name, define_t* out, deftb_t* ctx);
int MCTB_unload(deftb_t* ctx);

#endif