#ifndef NASM_FORMATTER_H_
#define NASM_FORMATTER_H_

#include <stdio.h>
#include <stdarg.h>
#include <stddef.h>
#include <symtab/symtab.h>

#define NASMFMT_FUNC_FMT   "_cpl_%s"
#define NASMFMT_STRING_FMT "_str_%li_"
#define NASMFMT_VTABLE_FMT "_cpl_vtable_%s"
#define NASMFMT_LABEL_FMT  "lb%llu"
#define NASMFMT_CONST_FMT  "%lli"

void        NASMFMT_emit(FILE* output, int newline, const char* fmt, ...);
const char* NASMFMT_format_array_value(const array_elem_info_t* elem, sym_table_t* smt, char* buffer, size_t size);
void        NASMFMT_emit_typed_value(FILE* output, long size, const array_elem_info_t* elem, sym_table_t* smt);

#ifndef EMIT_COMMAND
    #define EMIT_COMMAND(cmd, ...)      NASMFMT_emit(output, 1, cmd, ##__VA_ARGS__)
#endif

#ifndef EMIT_PART_COMMAND
    #define EMIT_PART_COMMAND(cmd, ...) NASMFMT_emit(output, 0, cmd, ##__VA_ARGS__)
#endif

#ifndef EMIT_DATA_LABEL
    #define EMIT_DATA_LABEL(label)      NASMFMT_emit(output, 1, "%s:", label)
#endif

#endif
