#ifndef POSITION_H_
#define POSITION_H_

#include <std/str.h>

typedef struct {
    long      line;
    long      column;
    string_t* file;
} file_position_t;

#endif
