#include <preproc/pp.h>

int PP_create_tmp_file(int src_fd) {
    char src_path[PP_PATH_MAX] = { 0 };
    char path[PP_PATH_MAX]     = { 0 };
    char dir_buf[PP_PATH_MAX]  = { 0 };

#if defined(__linux__)
    char link_path[64];
    snprintf(link_path, sizeof(link_path), "/proc/self/fd/%d", src_fd);
    ssize_t n = readlink(link_path, src_path, sizeof(src_path) - 1);
    if (n < 0) return -1;
    src_path[n] = 0;
#elif defined(__APPLE__)
    if (fcntl(src_fd, F_GETPATH, src_path) == -1) return -1;
#else
    return -1;
#endif

    if (src_path[0] != '/') return -1;
    strncpy(dir_buf, src_path, sizeof(dir_buf));
    dir_buf[sizeof(dir_buf) - 1] = 0;

    const char* dir = dirname(dir_buf);
    int w = snprintf(path, sizeof(path), "%s/.pp_tmp_XXXXXX", dir);
    if (w < 0 || (size_t)w >= sizeof(path)) return -1;

    int tmp_fd = mkstemp(path);
    if (tmp_fd < 0) return -1;
    unlink(path);

    fchmod(tmp_fd, 0644);

    int flags = fcntl(tmp_fd, F_GETFL);
    if (flags != -1) fcntl(tmp_fd, F_SETFL, flags | O_APPEND);

    fcntl(tmp_fd, F_SETFD, FD_CLOEXEC);
    return tmp_fd;
}

int PP_file_path_from_fp(FILE* fp, char* out, size_t out_sz) {
    if (!fp || !out || out_sz == 0) return 0;
    int fd = fileno(fp);
    if (fd < 0) return 0;
#if defined(__linux__)
    char link_path[64];
    snprintf(link_path, sizeof(link_path), "/proc/self/fd/%d", fd);
    ssize_t n = readlink(link_path, out, out_sz - 1);
    if (n < 0) return 0;
    out[n] = 0;
    return 1;
#elif defined(__APPLE__)
    if (fcntl(fd, F_GETPATH, out) == -1) return 0;
    return 1;
#else
    return 0;
#endif
}

char* PP_get_directive_from_line(char* line) {
    if (!line) return NULL;
    char* p = line;
    while (*p == ' ' || *p == '\t') p++;
    if (*p != '#') return NULL;
    p++;
    while (*p == ' ' || *p == '\t') p++;
    return *p ? p : NULL;
}

int PP_parse_include_arg(const char* p, char* out, size_t out_sz, int* is_system) {
    if (!p || !out || out_sz == 0 || !is_system) return 0;
    while (*p == ' ' || *p == '\t') p++;

    char end = 0;
    if (*p == '"') { *is_system = 0; end = '"'; p++; }
    else if (*p == '<') { *is_system = 1; end = '>'; p++; }
    else return 0;

    size_t i = 0;
    while (*p && *p != end) {
        if (i + 1 >= out_sz) return 0;
        out[i++] = *p++;
    }

    if (*p != end) return 0;
    out[i] = 0;
    return 1;
}

int PP_strip_colon_comments(const char* in, pp_cmt_state_t* st, char** out, size_t* out_cap) {
    if (!in || !st || !out || !out_cap) return 0;
    size_t n = strlen(in);
    if (!*out || *out_cap < n + 1) {
        size_t new_cap = n + 1;
        char* nb = (char*)realloc(*out, new_cap);
        if (!nb) return 0;
        *out = nb;
        *out_cap = new_cap;
    }

    size_t oi = 0;
    for (size_t i = 0; in[i]; i++) {
        char c = in[i];

        if (st->in_colon) {
            if (c == ':') st->in_colon = 0; 
            continue;
        }

        if (st->in_str) {
            (*out)[oi++] = c;
            if (st->esc)        st->esc    = 0;
            else if (c == '\\') st->esc    = 1;
            else if (c == '"')  st->in_str = 0;
            continue;
        }

        if (st->in_chr) {
            (*out)[oi++] = c;
            if (st->esc)        st->esc    = 0;
            else if (c == '\\') st->esc    = 1;
            else if (c == '\'') st->in_chr = 0;
            continue;
        }

        switch (c)  {
            case '"': { 
                st->in_str = 1; 
                (*out)[oi++] = c; 
                continue; 
            }
            case '\'': { 
                st->in_chr = 1; 
                (*out)[oi++] = c; 
                continue; 
            }
            case ':': { 
                st->in_colon = 1; 
                continue; 
            }
        }

        (*out)[oi++] = c;
    }

    (*out)[oi] = 0;
    return 1;
}

static int _is_ident_start(char c) {
    return (c == '_') || (c >= 'A' && c <= 'Z') || (c >= 'a' && c <= 'z');
}

static int _is_ident_char(char c) {
    return _is_ident_start(c) || (c >= '0' && c <= '9');
}

int PP_parse_define_arg(
    const char* p,
    char* name_out,  size_t name_sz,
    char* value_out, size_t value_sz
) {
    if (!p || !name_out || !name_sz || !value_out || !value_sz) return 0;

    while (*p == ' ' || *p == '\t') p++;
    if (!_is_ident_start(*p)) return 0;

    size_t i = 0;
    while (*p && _is_ident_char(*p)) {
        if (i + 1 >= name_sz) return 0;
        name_out[i++] = *p++;
    }

    name_out[i] = 0;

    if (*p == '(') {
        int depth = 0;
        while (*p && *p != '\n' && *p != '\r') {
            if (*p == '(') depth++;
            else if (*p == ')') {
                depth--;
                if (depth == 0) { 
                    p++; 
                    break; 
                }
            }

            p++;
        }

        if (depth != 0) return 0;
    }

    while (*p == ' ' || *p == '\t') p++;

    size_t j = 0;
    while (*p && *p != '\n' && *p != '\r') {
        if (j + 1 >= value_sz) return 0;
        value_out[j++] = *p++;
    }

    value_out[j] = 0;
    while (j > 0 && (value_out[j - 1] == ' ' || value_out[j - 1] == '\t')) {
        value_out[--j] = 0;
    }

    return 1;
}
