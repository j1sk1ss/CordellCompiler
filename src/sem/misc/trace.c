#include <sem/misc/trace.h>

int TRACE_init_trace(trace_t* trace) {
    list_init(&trace->messages);
    return 1;
}

static trace_message_t* _create_trace_message(trace_location_t* loc, const char* text) {
    trace_message_t* msg = (trace_message_t*)mm_malloc(sizeof(trace_message_t));
    if (!msg) return NULL;
    str_memcpy(&msg->location, loc, sizeof(trace_location_t));
    msg->message = create_string(text);
    return msg;
}

static int _unload_trace_message(trace_message_t* msg) {
    destroy_string(msg->message);
    mm_free(msg);
    return 1;
}

int TRACE_add_location(trace_t* trace, trace_location_t* loc, char* fmt, ...) {
    char buffer[512] = { 0 };
    va_list args;
    va_start(args, fmt);
    vsnprintf(buffer, sizeof(buffer), fmt, args);
    va_end(args);
    
    trace_message_t* message = _create_trace_message(loc, buffer);
    if (!message) return 0;

    list_add(&trace->messages, message);
    return 1;
}

int TRACE_unload_trace(trace_t* trace) {
    list_free_force_op(&trace->messages, (int (*)(void*))_unload_trace_message);
    return 1;
}
