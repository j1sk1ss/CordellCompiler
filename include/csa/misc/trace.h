#ifndef TRACE_H_
#define TRACE_H_

#include <stdarg.h>
#include <stdio.h>
#include <position.h>
#include <std/mm.h>
#include <std/mem.h>
#include <std/str.h>
#include <std/list.h>
#include <csa/misc/warns.h>

typedef struct {
    string_t*       message;
    file_position_t location;
} trace_message_t;

typedef long trace_id_t;

typedef enum {
    TRACE_SEVERITY_WARNING,
    TRACE_SEVERITY_ERROR,
    TRACE_SEVERITY_INFO
} trace_severity_t;

typedef struct {
    trace_id_t       id;
    trace_severity_t severity;
    trace_message_t* root;
    list_t           notes;
} trace_record_t;

typedef struct {
    trace_id_t next_id;
    list_t     records;
} trace_t;

#define TRACE_NO_ID ((trace_id_t)-1)

int TRACE_init_trace(trace_t* trace);
trace_id_t TRACE_create_root(trace_t* trace, trace_severity_t severity, file_position_t* loc, const char* fmt, ...);
int TRACE_add_note(trace_t* trace, trace_id_t id, file_position_t* loc, const char* fmt, ...);
int TRACE_is_empty(trace_t* trace);
int TRACE_print_and_free_trace(trace_t* t);
int TRACE_unload_trace(trace_t* trace);

#endif
