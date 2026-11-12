#include <asm/formatters/nasm_formatter.h>

static inline int _is_space(char c) {
    return c == ' ' || c == '\t';
}

static inline const char* _skip_spaces(const char* line) {
    while (_is_space(*line)) line++;
    return line;
}

static inline int _token_is(const char* start, size_t len, const char* token) {
    size_t i = 0;
    for (; i < len && token[i]; i++) {
        if (start[i] != token[i]) return 0;
    }

    return i == len && !token[i];
}

static inline void _split_first_token(const char* line, const char** rest, size_t* token_len) {
    const char* cursor = line;
    while (*cursor && !_is_space(*cursor)) cursor++;
    *token_len = (size_t)(cursor - line);
    *rest      = _skip_spaces(cursor);
}

static inline int _is_directive(const char* token, size_t len) {
    return _token_is(token, len, "section") ||
           _token_is(token, len, "global")  ||
           _token_is(token, len, "extern")  ||
           _token_is(token, len, "align")   ||
           _token_is(token, len, "%line");
}

static inline int _is_data_op(const char* token, size_t len) {
    return _token_is(token, len, "db")    ||
           _token_is(token, len, "dw")    ||
           _token_is(token, len, "dd")    ||
           _token_is(token, len, "dq")    ||
           _token_is(token, len, "times") ||
           _token_is(token, len, "resb")  ||
           _token_is(token, len, "resw")  ||
           _token_is(token, len, "resd")  ||
           _token_is(token, len, "resq");
}

static inline int _is_blank(const char* line) {
    return !(*_skip_spaces(line));
}

static inline int _is_label(const char* line) {
    size_t len = 0;
    while (line[len]) len++;
    while (len > 0 && _is_space(line[len - 1])) len--;
    return len > 0 && line[len - 1] == ':';
}

static inline void _emit_formatted_line(FILE* output, const char* line, int newline) {
    while (*line == '\n') {
        fputc('\n', output);
        line++;
    }

    const char* trimmed = _skip_spaces(line);
    if (_is_blank(trimmed)) {
        if (newline) fputc('\n', output);
        return;
    }

    if (*trimmed == ';' || _is_label(trimmed)) {
        fprintf(output, "%s", trimmed);
        if (newline) fputc('\n', output);
        return;
    }

    const char* rest = NULL;
    size_t first_len = 0;
    _split_first_token(trimmed, &rest, &first_len);

    if (_is_directive(trimmed, first_len)) {
        fprintf(output, "%.*s", (int)first_len, trimmed);
        if (*rest) fprintf(output, " %s", rest);
        if (newline) fputc('\n', output);
        return;
    }

    size_t second_len = 0;
    const char* data_args = NULL;
    _split_first_token(rest, &data_args, &second_len);
    if (second_len && _is_data_op(rest, second_len)) {
        fprintf(output, "%-24.*s %-7.*s %s", (int)first_len, trimmed, (int)second_len, rest, data_args);
        if (newline) fputc('\n', output);
        return;
    }

    if (*rest) fprintf(output, "    %-7.*s %s", (int)first_len, trimmed, rest);
    else fprintf(output, "    %.*s", (int)first_len, trimmed);
    if (newline) fputc('\n', output);
}

void NASMFMT_emit(FILE* output, int newline, const char* fmt, ...) {
    char buffer[1024];
    va_list args;
    va_start(args, fmt);
    vsnprintf(buffer, sizeof(buffer), fmt, args);
    va_end(args);
    _emit_formatted_line(output, buffer, newline);
}

const char* NASMFMT_format_array_value(const array_elem_info_t* elem, sym_table_t* smt, char* buffer, size_t size) {
    if (!elem) return "0";
    switch (elem->t) {
        case ARRAY_ELEM_STRING_TYPE: snprintf(buffer, size, NASMFMT_STRING_FMT, elem->s.s_id); break;
        case ARRAY_ELEM_FUNC_TYPE: {
            func_info_t fi;
            if (elem->s.f_id == NO_SYMBOL_ID || !FNTB_get_info_id(elem->s.f_id, &fi, &smt->f)) return "0";
            if (fi.flags.entry || fi.flags.vname) snprintf(buffer, size, "%s", fi.virt->body);
            else if (fi.flags.global || fi.flags.external) snprintf(buffer, size, "%s", fi.name->body);
            else snprintf(buffer, size, NASMFMT_FUNC_FMT, fi.virt->body);
            break;
        }
        case ARRAY_ELEM_VTABLE_TYPE: {
            type_info_t ti;
            vtable_info_t vi;
            if (
                elem->s.vt_id == NO_SYMBOL_ID                   ||
                !VTTB_get_info_id(elem->s.vt_id, &vi, &smt->vt) ||
                !TPTB_get_info_id(vi.t_id, &ti, &smt->t)
            ) return "0";
            snprintf(buffer, size, NASMFMT_VTABLE_FMT, ti.name->body);
            break;
        }
        case ARRAY_ELEM_LABEL_TYPE: snprintf(buffer, size, NASMFMT_LABEL_FMT, (unsigned long long)elem->s.value); break;
        default:                    snprintf(buffer, size, NASMFMT_CONST_FMT, elem->s.value);                     break;
    }
    return buffer;
}

void NASMFMT_emit_typed_value(FILE* output, long size, const array_elem_info_t* elem, sym_table_t* smt) {
    const char* op = size == 8 ? "dq" : size == 4 ? "dd" : size == 2 ? "dw" : "db";
    char buffer[256];
    const char* value = NASMFMT_format_array_value(elem, smt, buffer, sizeof(buffer));
    NASMFMT_emit(output, 1, "%s %s", op, value);
}
