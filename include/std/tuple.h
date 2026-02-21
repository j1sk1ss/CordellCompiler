#ifndef TUPLE_H_
#define TUPLE_H_

#include <std/mm.h>

typedef struct {
    long x;
    long y;
} int_tuple_t;

/*
Create a tuple object.
Params:
    - `x` - Current X value.
    - `y` - Current Y value.

Returns an object with passed values.
*/
int_tuple_t* inttuple_create(long x, long y);

/*
Free the input object.
Params:
    - `t` - Object to free.

Returns 1 if succeeds.
*/
int inttuple_free(int_tuple_t* t);

#endif