#include <csa/misc/trace.h>

int TRACE_init_trace(trace_t* trace) {
    trace->next_id = 1;
    list_init(&trace->records);
    return 1;
}

static trace_message_t* _create_trace_message_text(file_position_t* loc, const char* text) {
    trace_message_t* msg = (trace_message_t*)mm_malloc(sizeof(trace_message_t));
    if (!msg) return NULL;
    str_memcpy(&msg->location, loc, sizeof(file_position_t));
    msg->message = create_string(text);
    return msg;
}

static trace_message_t* _create_trace_message(file_position_t* loc, const char* fmt, va_list args) {
    char buffer[512] = { 0 };
    vsnprintf(buffer, sizeof(buffer), fmt, args);
    return _create_trace_message_text(loc, buffer);
}

static int _unload_trace_message(trace_message_t* msg) {
    destroy_string(msg->message);
    mm_free(msg);
    return 1;
}

static int _unload_trace_record(trace_record_t* record) {
    if (!record) return 1;
    if (record->root) _unload_trace_message(record->root);
    list_free_force_op(&record->notes, (int (*)(void*))_unload_trace_message);
    mm_free(record);
    return 1;
}

static trace_record_t* _find_trace_record(trace_t* trace, trace_id_t id) {
    foreach (trace_record_t* record, &trace->records) {
        if (record->id == id) return record;
    }

    return NULL;
}

static trace_id_t _add_root_va(
    trace_t* trace, trace_severity_t severity, file_position_t* loc, const char* fmt, va_list args
) {
    if (!trace || !loc || !fmt) return TRACE_NO_ID;

    trace_record_t* record = (trace_record_t*)mm_malloc(sizeof(trace_record_t));
    if (!record) return TRACE_NO_ID;

    record->root = _create_trace_message(loc, fmt, args);
    if (!record->root) {
        mm_free(record);
        return TRACE_NO_ID;
    }

    record->id       = trace->next_id++;
    record->severity = severity;
    list_init(&record->notes);

    if (!list_add(&trace->records, record)) {
        _unload_trace_record(record);
        return TRACE_NO_ID;
    }

    return record->id;
}

trace_id_t TRACE_create_root(trace_t* trace, trace_severity_t severity, file_position_t* loc, const char* fmt, ...) {
    va_list args;
    va_start(args, fmt);
    trace_id_t id = _add_root_va(trace, severity, loc, fmt, args);
    va_end(args);
    return id;
}

int TRACE_add_note(trace_t* trace, trace_id_t id, file_position_t* loc, const char* fmt, ...) {
    if (!trace || id == TRACE_NO_ID || !loc || !fmt) return 0;

    trace_record_t* record = _find_trace_record(trace, id);
    if (!record) return 0;

    va_list args;
    va_start(args, fmt);
    trace_message_t* message = _create_trace_message(loc, fmt, args);
    va_end(args);

    if (!message) return 0;
    if (!list_add(&record->notes, message)) {
        _unload_trace_message(message);
        return 0;
    }

    return 1;
}

int TRACE_is_empty(trace_t* trace) {
    return !list_size(&trace->records);
}

static inline const char* _severity_name(trace_severity_t severity) {
    switch (severity) {
        case TRACE_SEVERITY_ERROR:   return "ERROR";
        case TRACE_SEVERITY_INFO:    return "INFO";
        case TRACE_SEVERITY_WARNING:
        default:                     return "WARNING";
    }
}

static inline char* _format_location(file_position_t* loc) {
    static char buff[256] = { 0 };
    if (loc->file) snprintf(buff, sizeof(buff), "%s:%li:%li", loc->file->body, loc->line, loc->column);
    else snprintf(buff, sizeof(buff), "%li:%li", loc->line, loc->column);
    return buff;
}

static int _visible_len(const char* text) {
    return text ? str_strlen(text) : 0;
}

static void _print_wrapped(const char* first_prefix, const char* next_prefix, const char* text) {
    const int max_width = 120;
    const char* curr = text ? text : "";
    const char* prefix = first_prefix;

    do {
        while (*curr == ' ') curr++;

        int prefix_len = _visible_len(prefix);
        int limit = max_width - prefix_len;
        if (limit < 24) limit = 24;

        int len = _visible_len(curr);
        if (len <= limit) {
            fprintf(stdout, "%s%s\n", prefix, curr);
            return;
        }

        int cut = limit;
        for (int i = limit; i > 0; --i) {
            if (curr[i] == ' ') {
                cut = i;
                break;
            }
        }

        fprintf(stdout, "%s%.*s\n", prefix, cut, curr);
        curr += cut;
        prefix = next_prefix;
    } while (*curr);
}

static void _print_trace_message(
    const char* prefix, const char* next_prefix, const char* at_prefix, trace_message_t* message
) {
    _print_wrapped(prefix, next_prefix, message->message->body);
    fprintf(stdout, "%sat: %s\n", at_prefix, _format_location(&message->location));
}

static void _make_padding(char* buffer, int buffer_size, int width) {
    if (buffer_size <= 0) return;
    if (width >= buffer_size) width = buffer_size - 1;
    for (int i = 0; i < width; ++i) buffer[i] = ' ';
    buffer[width] = '\0';
}

static void _print_root(trace_record_t* record) {
    char prefix[64] = { 0 };
    char next_prefix[64] = { 0 };
    snprintf(prefix, sizeof(prefix), "[%s] ", _severity_name(record->severity));
    _make_padding(next_prefix, sizeof(next_prefix), _visible_len(prefix));
    _print_trace_message(prefix, next_prefix, "    ", record->root);
}

static void _print_note(trace_message_t* note) {
    _print_trace_message("    trace: ", "           ", "      ", note);
}

int TRACE_print_and_free_trace(trace_t* t) {
    foreach (trace_record_t* record, &t->records) {
        _print_root(record);
        foreach (trace_message_t* note, &record->notes) {
            _print_note(note);
        }
    }

    TRACE_unload_trace(t);
    return 1;
}

int TRACE_unload_trace(trace_t* trace) {
    list_free_force_op(&trace->records, (int (*)(void*))_unload_trace_record);
    return 1;
}
