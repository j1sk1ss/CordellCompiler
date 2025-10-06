#ifndef ALLIAS_H_
#define ALLIAS_H_

#include <std/mm.h>
#include <std/str.h>
#include <std/set.h>
#include <std/map.h>
#include <std/logg.h>

typedef struct {
    long  v_id;
    set_t owners;
    set_t delown;
} allias_t;

typedef struct {
    map_t allias;
} allias_map_t;

int ALLIAS_mark_owner(long v_id, long owner_id, allias_map_t* ctx);
int ALLIAS_add_owner(long v_id, long owner_id, allias_map_t* ctx);
int ALLIAS_get_owners(long v_id, set_t* out, allias_map_t* ctx);
int ALLIAS_unload(allias_map_t* ctx);

#endif