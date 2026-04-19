#ifndef TRACE_H_
#define TRACE_H_

#include <stdarg.h>
#include <stdio.h>
#include <position.h>
#include <std/mm.h>
#include <std/mem.h>
#include <std/str.h>
#include <std/list.h>
#include <sem/misc/warns.h>

typedef struct {
    string_t*       message;
    file_position_t location;
} trace_message_t;

typedef struct {
    list_t messages;
} trace_t;

int TRACE_init_trace(trace_t* trace);
int TRACE_add_location(trace_t* trace, file_position_t* loc, char* fmt, ...);
int TRACE_is_empty(trace_t* trace);
int TRACE_print_and_free_trace(trace_t* t);
int TRACE_unload_trace(trace_t* trace);

#endif
