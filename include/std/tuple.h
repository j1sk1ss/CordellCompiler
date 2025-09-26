#ifndef TUPLE_H_
#define TUPLE_H_

#include <std/mm.h>

typedef struct {
    long x;
    long y;
} int_tuple_t;

int_tuple_t* inttuple_create(long x, long y);
int inttuple_free(int_tuple_t* t);

#endif