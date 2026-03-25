#ifndef TRACE_H_
#define TRACE_H_

#include <stdarg.h>
#include <stdio.h>

#include <std/mm.h>
#include <std/mem.h>
#include <std/str.h>
#include <std/list.h>

typedef struct {
    long      line;
    long      column;
    string_t* file;
} trace_location_t;

typedef struct {
    string_t*        message;
    trace_location_t location;
} trace_message_t;

typedef struct {
    list_t messages;
} trace_t;

int TRACE_init_trace(trace_t* trace);
int TRACE_add_location(trace_t* trace, trace_location_t* loc, char* fmt, ...);
int TRACE_unload_trace(trace_t* trace);

#endif
