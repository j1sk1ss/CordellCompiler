#include <std/file.h>

#define PATH_BUF_SIZE 4096

static inline const char* _str_cstr(string_t* s) {
    if (!s) return NULL;
    if (s->body) return s->body;
    return s->head;
}

int is_same_file(string_t* a, string_t* b) {
    struct stat sa, sb;
    const char* pa = _str_cstr(a);
    const char* pb = _str_cstr(b);
    if (!pa || !pb) return 0;
    if (stat(pa, &sa)) return 0;
    if (stat(pb, &sb)) return 0;
    return sa.st_dev == sb.st_dev && sa.st_ino == sb.st_ino;
}

static int _get_parent_dir(string_t* path, char* out, size_t out_size) {
    const char* p;
    unsigned int len;
    int last_slash = -1;
    unsigned int i;

    if (!path || !out || !out_size) return 0;

    p = _str_cstr(path);
    if (!p) return 0;

    len = path->len(path);
    if (len + 1 > out_size) return 0;

    for (i = 0; i < len; i++) {
        if (p[i] == '/') last_slash = (int)i;
    }

    if (last_slash < 0) {
        if (out_size < 2) return 0;
        out[0] = '.';
        out[1] = 0;
        return 1;
    }

    if (last_slash == 0) {
        if (out_size < 2) return 0;
        out[0] = '/';
        out[1] = 0;
        return 1;
    }

    if ((size_t)last_slash + 1 > out_size) return 0;

    str_memcpy(out, p, last_slash);
    out[last_slash] = 0;
    return 1;
}

int is_same_dir(string_t* a, string_t* b) {
    char da[PATH_BUF_SIZE] = { 0 };
    char db[PATH_BUF_SIZE] = { 0 };
    struct stat sa, sb;
    if (!_get_parent_dir(a, da, sizeof(da))) return 0;
    if (!_get_parent_dir(b, db, sizeof(db))) return 0;
    if (stat(da, &sa)) return 0;
    if (stat(db, &sb)) return 0;
    return S_ISDIR(sa.st_mode) && S_ISDIR(sb.st_mode) &&
           sa.st_dev == sb.st_dev && sa.st_ino == sb.st_ino;
}