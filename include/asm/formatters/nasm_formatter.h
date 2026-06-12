#ifndef NASM_FORMATTER_H_
#define NASM_FORMATTER_H_

#include <stdarg.h>
#include <stddef.h>
#include <stdio.h>

static inline int NASMFMT_is_space(char c) {
    return c == ' ' || c == '\t';
}

static inline const char* NASMFMT_skip_spaces(const char* line) {
    while (NASMFMT_is_space(*line)) line++;
    return line;
}

static inline int NASMFMT_token_is(const char* start, size_t len, const char* token) {
    size_t i = 0;
    for (; i < len && token[i]; i++) {
        if (start[i] != token[i]) return 0;
    }

    return i == len && !token[i];
}

static inline void NASMFMT_split_first_token(const char* line, const char** rest, size_t* token_len) {
    const char* cursor = line;
    while (*cursor && !NASMFMT_is_space(*cursor)) cursor++;

    *token_len = (size_t)(cursor - line);
    *rest = NASMFMT_skip_spaces(cursor);
}

static inline int NASMFMT_is_directive(const char* token, size_t len) {
    return NASMFMT_token_is(token, len, "section") ||
           NASMFMT_token_is(token, len, "global")  ||
           NASMFMT_token_is(token, len, "extern")  ||
           NASMFMT_token_is(token, len, "align")   ||
           NASMFMT_token_is(token, len, "%line");
}

static inline int NASMFMT_is_data_op(const char* token, size_t len) {
    return NASMFMT_token_is(token, len, "db")   ||
           NASMFMT_token_is(token, len, "dw")   ||
           NASMFMT_token_is(token, len, "dd")   ||
           NASMFMT_token_is(token, len, "dq")   ||
           NASMFMT_token_is(token, len, "resb") ||
           NASMFMT_token_is(token, len, "resw") ||
           NASMFMT_token_is(token, len, "resd") ||
           NASMFMT_token_is(token, len, "resq");
}

static inline int NASMFMT_is_blank(const char* line) {
    return *NASMFMT_skip_spaces(line) == '\0';
}

static inline int NASMFMT_is_label(const char* line) {
    size_t len = 0;
    while (line[len]) len++;

    while (len > 0 && NASMFMT_is_space(line[len - 1])) len--;
    return len > 0 && line[len - 1] == ':';
}

static inline void NASMFMT_emit_formatted_line(FILE* output, const char* line, int newline) {
    while (*line == '\n') {
        fputc('\n', output);
        line++;
    }

    const char* trimmed = NASMFMT_skip_spaces(line);
    if (NASMFMT_is_blank(trimmed)) {
        if (newline) fputc('\n', output);
        return;
    }

    if (*trimmed == ';' || NASMFMT_is_label(trimmed)) {
        fprintf(output, "%s", trimmed);
        if (newline) fputc('\n', output);
        return;
    }

    const char* rest = NULL;
    size_t first_len = 0;
    NASMFMT_split_first_token(trimmed, &rest, &first_len);

    if (NASMFMT_is_directive(trimmed, first_len)) {
        fprintf(output, "%.*s", (int)first_len, trimmed);
        if (*rest) fprintf(output, " %s", rest);
        if (newline) fputc('\n', output);
        return;
    }

    size_t second_len = 0;
    const char* data_args = NULL;
    NASMFMT_split_first_token(rest, &data_args, &second_len);
    if (second_len && NASMFMT_is_data_op(rest, second_len)) {
        fprintf(output, "%-24.*s %-7.*s %s", (int)first_len, trimmed, (int)second_len, rest, data_args);
        if (newline) fputc('\n', output);
        return;
    }

    if (*rest) fprintf(output, "    %-7.*s %s", (int)first_len, trimmed, rest);
    else fprintf(output, "    %.*s", (int)first_len, trimmed);
    if (newline) fputc('\n', output);
}

#define NASM_FORMATTER_EMIT_BUFFER_SIZE 1024

static inline void NASMFMT_emit_command(FILE* output, const char* fmt, ...) {
    char buffer[NASM_FORMATTER_EMIT_BUFFER_SIZE];
    va_list args;
    va_start(args, fmt);
    vsnprintf(buffer, sizeof(buffer), fmt, args);
    va_end(args);

    NASMFMT_emit_formatted_line(output, buffer, 1);
}

static inline void NASMFMT_emit_part_command(FILE* output, const char* fmt, ...) {
    char buffer[NASM_FORMATTER_EMIT_BUFFER_SIZE];
    va_list args;
    va_start(args, fmt);
    vsnprintf(buffer, sizeof(buffer), fmt, args);
    va_end(args);

    NASMFMT_emit_formatted_line(output, buffer, 0);
}

#ifndef EMIT_COMMAND
#define EMIT_COMMAND(cmd, ...) NASMFMT_emit_command(output, cmd, ##__VA_ARGS__)
#endif

#ifndef EMIT_PART_COMMAND
#define EMIT_PART_COMMAND(cmd, ...) NASMFMT_emit_part_command(output, cmd, ##__VA_ARGS__)
#endif

#endif
