#ifndef ALLIAS_H_
#define ALLIAS_H_

#include <std/mm.h>
#include <std/str.h>
#include <std/set.h>
#include <std/logg.h>

typedef struct allias {
    long           v_id;
    set_t          owners;
    struct allias* next;
} allias_t;

typedef struct {
    allias_t* h;
} allias_map_t;

int ALLIAS_add_owner(long v_id, long owner_id, allias_map_t* ctx);
int ALLIAS_get_owners(long v_id, set_t* out, allias_map_t* ctx);
int ALLIAS_unload(allias_map_t* ctx);

#endif