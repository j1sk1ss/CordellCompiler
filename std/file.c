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

static int _count_components(const char* path) {
    int count = 0, in_component = 0;
    if (!path) return -1;
    while (*path) {
        if (*path == '/') in_component = 0; 
        else if (!in_component) {
            count++;
            in_component = 1;
        }

        path++;
    }

    return count;
}

static int _common_components(const char* a, const char* b) {
    const char* pa = a;
    const char* pb = b;
    int common = 0;

    if (!a || !b) return -1;

    while (*pa == '/') pa++;
    while (*pb == '/') pb++;

    while (*pa && *pb) {
        const char* a_start = pa;
        const char* b_start = pb;
        size_t a_len = 0, b_len = 0;

        while (pa[a_len] && pa[a_len] != '/') a_len++;
        while (pb[b_len] && pb[b_len] != '/') b_len++;
        if (
            a_len != b_len || 
            str_memcmp(a_start, b_start, a_len)
        ) break;
        
        common++;

        pa += a_len;
        pb += b_len;

        while (*pa == '/') pa++;
        while (*pb == '/') pb++;
    }

    return common;
}

int get_dir_distance(string_t* a, string_t* b) {
    char da[PATH_BUF_SIZE] = { 0 };
    char db[PATH_BUF_SIZE] = { 0 };

    int depth_a, depth_b, common;
    if (
        !_get_parent_dir(a, da, sizeof(da)) ||
        !_get_parent_dir(b, db, sizeof(db))
    ) return -1;

    depth_a = _count_components(da);
    depth_b = _count_components(db);

    if (
        (depth_a < 0 || depth_b < 0) ||
        ((da[0] == '/') != (db[0] == '/'))
    ) return -1;

    common = _common_components(da, db);
    if (common < 0) return -1;
    return (depth_a - common) + (depth_b - common);
}