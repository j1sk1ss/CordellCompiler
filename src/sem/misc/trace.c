#include <sem/misc/trace.h>

int TRACE_init_trace(trace_t* trace) {
    list_init(&trace->messages);
    return 1;
}

static trace_message_t* _create_trace_message(file_position_t* loc, const char* text) {
    trace_message_t* msg = (trace_message_t*)mm_malloc(sizeof(trace_message_t));
    if (!msg) return NULL;
    str_memcpy(&msg->location, loc, sizeof(file_position_t));
    msg->message = create_string(text);
    return msg;
}

static int _unload_trace_message(trace_message_t* msg) {
    destroy_string(msg->message);
    mm_free(msg);
    return 1;
}

int TRACE_add_location(trace_t* trace, file_position_t* loc, char* fmt, ...) {
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

int TRACE_is_empty(trace_t* trace) {
    return !list_size(&trace->messages);
}

static inline char* _format_location(file_position_t* loc) {
    static char buff[256] = { 0 };
    if (loc->file) snprintf(buff, sizeof(buff), "[%s:%li:%li]", loc->file->body, loc->line, loc->column);
    else snprintf(buff, sizeof(buff), "[%li:%li]", loc->line, loc->column);
    return buff;
}

int TRACE_print_and_free_trace(trace_t* t) {
    list_iter_t it;
    list_iter_tinit(&t->messages, &it);
    trace_message_t* msg = list_iter_prev(&it);
    if (msg) SEMANTIC_WARNING(" %s %s ", _format_location(&msg->location), msg->message->body);
    while ((msg = list_iter_prev(&it))) {
        EMMIT_MESSAGE(" %s     %s ", _format_location(&msg->location), msg->message->body);
    }

    TRACE_unload_trace(t);
    return 1;
}

int TRACE_unload_trace(trace_t* trace) {
    list_free_force_op(&trace->messages, (int (*)(void*))_unload_trace_message);
    return 1;
}
