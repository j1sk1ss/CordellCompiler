#ifndef PP_H_
#define PP_H_

#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>
#include <limits.h>
#include <libgen.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include <sys/stat.h>
#include <std/str.h>
#include <std/stack.h>
#include <preproc/mctb.h>
#include <preproc/directives.h>

#define IS_PP_DERICTIVE(s, der) !strncmp(s, der, sizeof(der) - 1) &&     \
                          (                                              \
                              !s[sizeof(der) - 1] ||                     \
                              isspace((unsigned char)s[sizeof(der) - 1]) \
                          )
#define PP_GET_DERICTIVE_OFF(der) sizeof(der) - 1
#define PP_MV_LINE_DIR(s, der) s + (PP_GET_DERICTIVE_OFF(der))
#define PP_PATH_MAX 256

/*
Create and open temp file near to the source fd file.
Params:
    - `src_fd` - Source FD.

Returns FD of a tmp file or -1.
*/
int PP_create_tmp_file(int src_fd);

/*
Grub a path from the fp.
Params:
    - `fp` - File pointer.
    - `out` - Output file path.
    - `out_sz` - Max output file path size.

Returns 1 if succeeds, otherwise will return 0.
*/
int PP_file_path_from_fp(FILE* fp, char* out, size_t out_sz);

/*
Get a derictive from the line.
Params:
    - `line` - Line to consider.

Returns a directive start symbol.
*/
char* PP_get_directive_from_line(char* line);

/*
Parse the include directive.
Include can be:
    - #include "file.cpl"
    - #include <file.cpl> [WIP]

Params:
    - `p` - Directive line.
    - `out` - Include path output.
    - `out_sz` - Include path max size.
    - `is_system` - Is this is a system lib? <>

Returns 1 if succeeds, otherwise will return 0.
*/
int PP_parse_include_arg(const char* p, char* out, size_t out_sz, int* is_system);

typedef struct {
    int in_colon;   /* In comment section */
    int in_str;     /* In string          */
    int in_chr;     /* In character       */
    int esc;        /* Backslash          */
} pp_cmt_state_t;

/*
Clean the input buffer from a comment section.
Params:
    - `in` - Input buffer.
    - `st` - Comment state.
    - `out` - Output clean line.
    - `out_cap` - Output line max size.

Returns 0 if fails, otherwise will return 1.
*/
int PP_strip_colon_comments(const char* in, pp_cmt_state_t* st, char** out, size_t* out_cap);

/*
Parse the 'define'-like directive.
Params:
    - `p` - Input line.
    - `name_out` - Variable/Macro name.
    - `name_sz` - Max variable name size.
    - `value_out` - Value body.
    - `value_sz` - Max value body size.

Returns 1 if succeeds. Otherwise will return 0.
*/
int PP_parse_define_arg(
    const char* p,
    char* name_out,  size_t name_sz,
    char* value_out, size_t value_sz
);

typedef struct {
    const char* bpath; /* Basic include path */
} finder_ctx_t;

int PP_perform(int fd, finder_ctx_t* fctx);

#endif
